forest
======

Compute edit script to convert one classification into another.

Algorithm described in "An edit script for taxonomic classifications" [doi:10.1186/1471-2105-6-208](http://dx.doi.org/10.1186/1471-2105-6-208)

#### Example

The files ncbi_animal.gml and new_animal.gml contain simplified NCBI and "new" animal classifications, respectively:

![NCBI](https://raw.github.com/rdmpage/forest/master/example/1471-2105-6-208-1.jpg)

![New](https://raw.github.com/rdmpage/forest/master/example/1471-2105-6-208-2.jpg)

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

![Edit](https://raw.github.com/rdmpage/forest/master/example/1471-2105-6-208-7.jpg)


#### Dependencies
The program uses the Graph Template Library (GTL) which is available from [http://www.fim.uni-passau.de/fileadmin/files/lehrstuhl/brandenburg/projekte/gtl/GTL-1.2.4-lgpl.tar.gz](http://www.fim.uni-passau.de/fileadmin/files/lehrstuhl/brandenburg/projekte/gtl/GTL-1.2.4-lgpl.tar.gz). There is a copy of this file in the vendor folder.

#### Building
If you are building from this repository you will need to do the standard things:

* aclocal
* autoconf
* automake
* ./configure
* make

## Displaying tree difference

The program html will generate a web page showing the differences between the trees. For example, given two classifications of whales, you can generate an edit script:

    ./forest MSW/Cetacea.gml MSW/GBIF-Cetacea.gml > cet.txt

The file Cet.txt contains the edit script, which you can then pass (along with the trees) to html:

    ./html MSW/Cetacea.gml MSW/GBIF-Cetacea.gml cet.txt > cet.html

If you open cet.html in a web browser you will have an interactive viewer of the differences between the two trees. You can scroll up and down in both trees. Clicking on a node in one tree will force the corresponding node (if it exists) in the other tree to scroll into view.