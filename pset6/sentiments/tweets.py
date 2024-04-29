#!/usr/bin/env python3

import os
import sys

from analyzer import Analyzer
from termcolor import colored
import helpers

def main():
    # ensure proper usage
    if len(sys.argv) != 2:
        sys.exit("Usage: ./tweet @name")
    elif not sys.argv[1].startswith('@'):
        sys.exit("Usage: ./tweet @name")
    
    # username
    name = sys.argv[1][1:]
    # print("{}".format(name))

    # absolute paths to lists 
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)
    
    # check if the list of tweets is received
    tweets = helpers.get_user_timeline(name, 50)
    if tweets == None:
        sys.exit()
    
    for tweet in tweets:
        score = analyzer.analyze(tweet)
        if score > 0.0:
            print(colored("{}".format(tweet), "green"))
        elif score < 0.0:
            print(colored("{}".format(tweet), "red"))
        else:
            print(colored("{}".format(tweet), "yellow"))
    

if __name__ == "__main__":
    main()