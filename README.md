forest
======

Compute edit script to convert one classification into another

Algorithm decsribed in "An edit script for taxonomic classifications" [doi:10.1186/1471-2105-6-208](http://dx.doi.org/10.1186/1471-2105-6-208)

#### Example

The files ncbi_animal.gml and new_animal.gml contain simplified NCBI and "new" animal classifications, respectively:

![https://raw.github.com/rdmpage/forest/master/example/1471-2105-6-208-1.jpg]

![https://raw.github.com/rdmpage/forest/master/example/1471-2105-6-208-2.jpg]

Comparing these two classifications generates an "edit script" listing the operations needed to transform one tree into the other:

	delete node Pseudocoelemata
	delete node Coelomata
	delete node Protostomia
	delete node Acoelomata
	insert node Ecdysozoa
	insert node Lophotrochozoa
	insert node Protostomia
	insert edge Bilateria -> Deuterostomia
	insert edge Bilateria -> Protostomia
	insert edge Ecdysozoa -> Nematoda
	insert edge Ecdysozoa -> Arthropoda
	insert edge Lophotrochozoa -> Annelida
	insert edge Lophotrochozoa -> Brachiopoda
	insert edge Lophotrochozoa -> Bryozoa
	insert edge Lophotrochozoa -> Mollusca
	insert edge Lophotrochozoa -> Nemertea
	insert edge Lophotrochozoa -> Platyhelminthes
	insert edge Protostomia -> Lophotrochozoa
	insert edge Protostomia -> Ecdysozoa

![https://raw.github.com/rdmpage/forest/master/example/1471-2105-6-208-7.jpg]

