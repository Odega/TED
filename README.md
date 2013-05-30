TED
===

INF1510 - Prosjekt
V2013

Av:
  Mikel Von Krogh,
  Benjamin Thomas,
  Mats Øverland,
  Magnus Smogeli,
  Eirik Ødegaard

Forklaring av kode:
Hoveddelen består av en switch state i loopen, hvor alle bjørnens kommandoer blir utført.
Hver state har en komponent, hvis trigges, sendes videre til neste state. Derimot hvis det ikke skjer
noe over et tidsrom, sender staten tilbake til forrige, hvor den til slutt ender opp i IDLE (Hvor den venter på å
oppfatte noen i umiddelbar nærhet).

I hver state, om komponenten blir trigget, sender den med nr til staten den er i (stateKey) til en metode som
tar imot og spiller av et tilfeltig spor som passer til konteksten.
