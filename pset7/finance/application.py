from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# INDEX!!!!!!!!!!!!!!!!!!!!!!
@app.route("/")
@login_required
def index():
    cash_balance = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    balance = cash_balance[0]['cash']
    total_user = float(balance)
    user_shares = db.execute("SELECT symbol, shares FROM stock WHERE id = :id", id = session["user_id"])
    rows = []
    for us in user_shares:
        symbol = us['symbol']
        shares = us['shares']
        stock = lookup(symbol)
        if stock != None:
            name = stock['name']
            price = stock['price']
            total = price * shares
            total_user += total
        row = (symbol, name, shares, usd(price), usd(total))
        rows.append(row)
    
    # print(rows)        
    return render_template("index.html", rows = rows, cash = usd(balance), total = usd(total_user))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy_form.html")
    else:
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Please fill all forms")
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        if not isinstance(shares, int) or shares < 1:
            return apology("Enter a positive number")
        result = lookup(symbol)
        if result == None:
            return apology("symbol", "not found")
        else:
            name = result['name']
            price = result['price']
            symbol = result['symbol']
            user_id = session["user_id"]
            rows = db.execute("SELECT cash FROM users WHERE id = :id", id=user_id)
            cash = float(rows[0]["cash"])
            new_cash = cash - shares * price
            if new_cash < 0:
                return apology("You don't have", "enough money")
            else:
                stock_ins = db.execute("INSERT INTO track (id, symbol, shares, price) VALUES(:id, :symbol, :shares, :price)",
                        id = user_id, symbol = symbol, shares = shares, price = float(shares * price))
                cash_upd = db.execute("UPDATE users SET cash = :cash WHERE id= :id", cash = new_cash, id = user_id)
                total = db.execute("SELECT shares FROM stock  WHERE id = :id AND symbol = :symbol", id = user_id, symbol = symbol)
                
                if not total:
                    db.execute("INSERT INTO stock (id, symbol, shares) VALUES (:id, :symbol, :shares)", id = user_id, symbol = symbol,
                                shares = shares)
                else:
                    shares_upd = total[0]['shares'] + shares
                    db.execute("UPDATE stock SET shares = :shares WHERE id = :id AND symbol = :symbol", shares = shares_upd, id = user_id, 
                                symbol = symbol)
                return redirect(url_for("index"))
            

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    track = db.execute("SELECT * FROM track WHERE id = :id", id = session["user_id"])
    rows = []
    for tr in track:
        symbol = tr['symbol']
        stock = lookup(symbol)
        if stock != None:
            name = stock['name']
        else:
            name = "Not found"
        shares = tr['shares']
        price = tr['price']
        date = tr['date']
        row = (symbol, name, shares, usd(price), date)
        rows.append(row)
        
    return render_template("history.html", rows = rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote_form.html")
    else:
        symbol = request.form.get("symbol")
        result = lookup(symbol)
        if result == None:
            return apology("symobol", "not found")
        else:
            name = result['name']
            price = usd(result['price'])
            symbol = result['symbol']
            return render_template("quote.html", name = name, price = price, symbol = symbol)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # forget any user_id
    session.clear()
    
    #если пользователь просто открыл страницу, отображаем страницу регистрации
    if request.method == "GET":
        return render_template("register.html")
    #если заполнил форму и нажал submit, включается POST
    else:
        if not request.form.get("username") or not request.form.get("password") or not request.form.get("confirmation"):
            return apology("you should fill", "all forms")

        # ensure password it is correct
        elif len(request.form.get('password')) < 4:
            return apology("Your password is", "too short")
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation", "don't match")
        else:
            uname = request.form.get("username")
            # pw = pwd_context.encrypt(request.form.get("password"))
            pw = pwd_context.hash(request.form.get("password"))
            validation = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username = uname, hash = pw)
            if not validation:
                return apology("Ooops", "You are not registered")
            else:
                row = db.execute("SELECT id FROM users WHERE username = :username", username=uname)
                session["user_id"] = row[0]["id"]
                return redirect(url_for("index"))
                
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    rows = db.execute("SELECT symbol, shares FROM stock WHERE id = :id", id = session["user_id"])
    if not rows:
        return apology("You don't have", "anything to sell")
    if request.method == "GET":
        return render_template("sell_form.html", rows = rows)
    else:
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("Please fill all forms")
        symbol = request.form.get("symbol")
        shares_tosell = int(request.form.get("shares"))
        user_id = session["user_id"]
        if not isinstance(shares_tosell, int) or shares_tosell < 1:
            return apology("Enter a positive number")
        result = lookup(symbol)
        if result == None:
            return apology("symbol", "not found")
        price = result['price']
        summa = float(shares_tosell * price)
        number_request = db.execute("SELECT shares FROM stock WHERE id = :id AND symbol = :symbol", id = user_id, 
                    symbol = symbol)
        number = int(number_request[0]['shares'])
        cash_request = db.execute("SELECT cash FROM users WHERE id = :id", id = user_id)
        cash = float(cash_request[0]["cash"])
        new_cash = cash + summa
        if number < shares_tosell:
            return apology("Not enough", "shares")
        db.execute("INSERT INTO track (id, symbol, shares, price) VALUES(:id, :symbol, :shares, :price)",
                id = user_id, symbol = symbol, shares = (0-shares_tosell), price = summa)
        db.execute("UPDATE users SET cash = :cash WHERE id= :id", cash = new_cash, id = user_id)
        if number == shares_tosell:
             db.execute("DELETE FROM stock WHERE symbol = :symbol", symbol = symbol)
        else:
            db.execute("UPDATE stock SET shares = :shares WHERE id = :id AND symbol = :symbol", shares = number - shares_tosell,
                id = user_id, symbol = symbol)
        return redirect(url_for("index"))
