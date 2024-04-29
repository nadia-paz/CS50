import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))
    
@app.route("/articles")
def articles():
    """Look up articles for geo."""

    # TODO
    
    geo = request.args.get("geo")
    
    if not geo:
        raise RuntimeError("GEO not found")
    
    articles = lookup(geo)
    if len(articles) < 5:
        return jsonify(articles)
    else:
        artlicles1 = []
        for x in range (5):
            # articles1 += artiles[x] here doesn't work :o)
            artlicles1.append(articles[x])
        return jsonify(artlicles1)

@app.route("/search")
def search():
    """Search for places that match query."""

    # TODO
    q = request.args.get("q")
    
    if not q:
        raise RuntimeError("No query included")
    search = []
    if q.isdigit():
        search = db.execute("SELECT * FROM places WHERE postal_code LIKE :q", q=q + "%")
    else:
        s1 = db.execute("SELECT * FROM places WHERE place_name LIKE :q", q = q + "%")
        if s1:
            # search.append(s1)
            #append adds the array inside the array :o) [[s1]]
            search += s1
        s2 = db.execute("SELECT * FROM places WHERE admin_name1 LIKE :q", q = q + "%")
        if s2:
            search += s2
        s3 = db.execute("SELECT * FROM places WHERE admin_name2 LIKE :q", q = q + "%")
        if s3:
            search += s3
        s4 = db.execute("SELECT * FROM places WHERE admin_name3 LIKE :q", q = q + "%")
        if s4:
            search += s4
        s5 = db.execute("SELECT * FROM places WHERE admin_code1 LIKE :q", q = q + "%")
        if s5:
            search += s5
    return jsonify(search)

@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)
