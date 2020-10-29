#!/usr/bin/python

from html.parser import HTMLParser
from collections import deque
import requests

def getAtt(attrs, theAttr):
    for attr in attrs:
        if attr[0] == theAttr:
            return attr[1]
    return None
    

# website = urllib2.urlopen("https://pokemondb.net/pokedex/all")
parser = HTMLParser()
class MyHTMLParser(HTMLParser):
    in_table: bool = False
    in_tbody: bool = False
    in_row: bool = False
    next_td: bool = False

    current_field: str = ""
    line: str = ""
    i: int = 1
    closing: bool = False
    print_header: bool = False
    header: deque = deque([ "Index No.", "Name", "Type", "Total", "HP", "Attack", "Defense", "Sp.Atk", "Sp.Def", "Speed"])

    def handle_starttag(self, tag, attrs):
        if not self.in_table:
            _id = getAtt(attrs, "id")
            if tag == "table" and _id == "pokedex":
                self.in_table = True
        else:
            if tag == "tbody":
                self.in_tbody = True
            if tag == "tr":
                self.in_row = True

    def handle_endtag(self, tag):
        if tag == "table":
            self.in_table = False
        if self.in_tbody and tag == "tbody":
            in_tbody = False
        if self.in_row and tag == "tr":
            print(self.line)
            self.line = ""
            self.i += 1
            if (not self.closing) and self.i > 10000:
                raise StopIteration
        if self.in_row and tag == "td":
            if self.print_header:
                current = self.header.popleft()
                self.line += current + ": " + self.current_field + ","
                self.header.append(current)
            else:
                self.line += self.current_field + ","
            self.current_field = ""

    def handle_data(self, data):
        if self.in_tbody and self.in_row:
            data = data.strip()

            if data == "":
                return
            elif self.current_field == "":
                self.current_field = data
            else:
                self.current_field += ";" + data
            
        

def main():
    remote_page = "https://pokemondb.net/pokedex/all"
    response = requests.get(remote_page)

    if not response.ok:
        print("Error trying to fetch remote content from {}: Error code: {}".format(remote_page, response.status_code))
        return None

    parser = MyHTMLParser()
    try:
        parser.feed(response.content.decode("utf-8"))
    except StopIteration as e:
        return

if __name__ == "__main__":
    main()
