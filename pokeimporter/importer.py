#!/usr/bin/python
from bs4 import BeautifulSoup
from collections import deque
import requests
import os.path

def try_get(arr, i):
    if i < len(arr):
        return arr[i]
    else:
        return ""

class PokemonData:
    dexNo: int
    name: str
    types: list
    stats: int
    hp: int
    atk: int
    spAtk: int
    deff: int
    spDeff: int
    spe: int
    def to_string(self):
        return "{};{};{};{};{};{};{};{};{};{};{}".format(
            self.dexNo,
            self.name,
            try_get(self.types, 0),
            try_get(self.types, 1),
            self.stats,
            self.hp,
            self.atk,
            self.spAtk,
            self.deff,
            self.spDeff,
            self.spe
            )
def main():
    pokemon_csv = open("pokemon.csv", "x")
    pokemon_detail_csv = open("pokemon_detail.csv", "x")
    pokemon_pokedex_entries_csv = open("pokemon_pokedex_entries.csv", "x")
    html_repository = ".cache/"
    if not os.path.exists(html_repository):
        os.makedirs(html_repository)

    hostname = "https://pokemondb.net"
    response = requests.get(hostname + "/pokedex/all")

    if not response.ok:
        print("Error trying to fetch remote content from {}: Error code: {}".format(hostname, response.status_code))
        return None

    soup = BeautifulSoup(response.content.decode("utf-8"), "html.parser")
    for tr in soup.select("table#pokedex > tbody > tr"):
        tds = tr.find_all('td')
        pkm = PokemonData()
        pkm.dexNo = int(tds[0].select(".infocard-cell-data")[0].get_text())
        pkm.name = tds[1].get_text()
        pkm.types = list( map(lambda e: e.get_text(), tds[2].find_all("a")) )
        pkm.stats = int(tds[3].get_text())
        pkm.hp = int(tds[3].get_text())
        pkm.atk = int(tds[4].get_text())
        pkm.spAtk = int(tds[5].get_text())
        pkm.deff = int(tds[6].get_text())
        pkm.spDeff = int(tds[7].get_text())
        pkm.spe = int(tds[8].get_text())

        # That's all we need for the main file. Write it.
        pokemon_csv.write(pkm.to_string())
        pokemon_csv.write("\n")
        pokemon_csv.flush()

        # Here we can get more detailed info
        filename = html_repository + str(pkm.dexNo) + ".html"
        html_content = ""
        ps = None
        if os.path.exists(filename):
            print('arquivo ja existe ' + filename)
            with open(filename, 'r') as istream:
                ps = BeautifulSoup(istream, "html.parser")
        else:
            url = tds[1].find("a")["href"]
            response = requests.get(hostname + url)
            content = response.content.decode("utf-8")
            ps = BeautifulSoup(content, "html.parser")
            with open(filename, 'x') as ostream:
                ostream.write(content)

        # Get pokedex descriptions
        pokedexEntriesTitle = ps.find("h2", string="Pokédex entries")
        pokedexEntriesTable = pokedexEntriesTitle.find_next_sibling("div").table
        
        pokemon_pokedex_entries_csv.write(str(pkm.dexNo))
        pokemon_pokedex_entries_csv.write(";")

        for dexEntry in pokedexEntriesTable.tbody.find_all("tr"):
            games = ",".join(map(lambda e: e.get_text(), dexEntry.th.find_all('span')))
            descr = dexEntry.td.get_text()
            pokemon_pokedex_entries_csv.write("({}){};".format(games, descr))
        pokemon_pokedex_entries_csv.write("\n")
        pokemon_pokedex_entries_csv.flush()

    pokemon_csv.close()
    pokemon_detail_csv.close()
    pokemon_pokedex_entries_csv.close()
  
if __name__ == "__main__":
    main()
