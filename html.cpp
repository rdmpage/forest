// Draw tree

#include <GTL/graph.h>
#include <GTL/dfs.h>

#include "mygraph.h"
#include "mytree.h"

#include "tokenise.h"


#include <stack>


// traverse tree, output as HTML nested list
// All purpose traversal of tree (ugly)
class mydfs : public dfs
{
public:
	mydfs () : dfs () { this_tree = 0;  };
	
	virtual void set_tree_number(int n) { this_tree = n; }

	virtual void entry_handler (graph &G, node &n, node &f)
	{		
		//cout << "entry\n";
		
		cout << "<ul>";
		cout << "<li>";
		
		string colour = dynamic_cast<MyTree*>(&G)->get_node_colour(n);
		string text = dynamic_cast<MyTree*>(&G)->get_node_label(n);
		
		if (colour == "deleted")
		{
			cout << "<span class=\"deleted\">";
			cout << text;
			cout << "</span>";
		}
		
		if (colour == "inserted")
		{
			cout << "<span class=\"inserted\">";
			cout << text;
			cout << "</span>";
		}
		
		if ((colour == "moved") || (colour == "white"))
		{
			if (colour == "moved")
			{
				cout << "<span class=\"moved\">";
			}
			else
			{
				cout << "<span class=\"shared\">";			
			}
			cout << "<a name=\"" << text << this_tree << "\""
				<< " href=\"#" << text;
			if (this_tree == 1) 
			{
				cout << (this_tree + 1);
			}
			else
			{
				cout << (this_tree - 1);
			}
			cout << "\">";
			cout << text;
			cout << "</a>";
			cout << "</span>";
		}		

		cout << "</li>";
		
	}
	
	virtual void leave_handler (graph &G, node &n, node &f)
	{
		cout << "</ul>";		
	}
	
	/*
	virtual void before_recursive_call_handler (graph &G, node &n, edge &e)
	{
		cout << "before\n";
	}
	
	virtual void after_recursive_call_handler (graph &G, node &n, edge &e)
	{
		cout << "after\n";
	}
	*/
	
	
protected:
	int this_tree;
};



int main( int argc, char *argv[] ) 
{
	node v, w;

	if (argc < 4)
	{
		cout << "Usage: forest <file-name-1> <file-name-2> <script-file-name>" << endl;
		exit(1);
	}

	const char* filename1 = argv[1];
	MyTree T1;
	T1.read_labels_as_weights();
	GML_error err  = T1.load (filename1);
	if (err.err_num != GML_OK)
	{
		cerr << "Error (" << err.err_num << ") loading graph from file \"" << filename1 << "\"";
		switch (err.err_num)
		{
			case GML_FILE_NOT_FOUND: cerr << "A file with that name doesn't exist."; break;
			case GML_TOO_MANY_BRACKETS: cerr << "A mismatch of brackets was detected, i.e. there were too many closing brackets (])."; break;
			case GML_OPEN_BRACKET: cerr << "Now, there were too many opening brackets ([)";  break;
			case GML_TOO_MANY_DIGITS: cerr << "The number of digits a integer or floating point value can have is limited to 1024, this should be enough :-)";  break;
			case GML_PREMATURE_EOF: cerr << "An EOF occured, where it wasn't expected, e.g. while scanning a string."; break;
			case GML_SYNTAX: cerr << "The file isn't a valid GML file, e.g. a mismatch in the key-value pairs."; break;
			case GML_UNEXPECTED: cerr << "A character occured, where it makes no sense, e.g. non-numerical characters"; break;
			case GML_OK: break;
		}
		cerr << endl;
		exit(1);
	}
	else
	{
		cerr << filename1 << " has " << T1.number_of_nodes() << " nodes" << endl;
	}
	
	const char* filename2 = argv[2];
	MyTree T2;
	T2.read_labels_as_weights();
	err  = T2.load (filename2);
	if (err.err_num != GML_OK)
	{
		cerr << "Error (" << err.err_num << ") loading graph from file \"" << filename2 << "\"";
		switch (err.err_num)
		{
			case GML_FILE_NOT_FOUND: cerr << "A file with that name doesn't exist."; break;
			case GML_TOO_MANY_BRACKETS: cerr << "A mismatch of brackets was detected, i.e. there were too many closing brackets (])."; break;
			case GML_OPEN_BRACKET: cerr << "Now, there were too many opening brackets ([)";  break;
			case GML_TOO_MANY_DIGITS: cerr << "The number of digits a integer or floating point value can have is limited to 1024, this should be enough :-)";  break;
			case GML_PREMATURE_EOF: cerr << "An EOF occured, where it wasn't expected, e.g. while scanning a string."; break;
			case GML_SYNTAX: cerr << "The file isn't a valid GML file, e.g. a mismatch in the key-value pairs."; break;
			case GML_UNEXPECTED: cerr << "A character occured, where it makes no sense, e.g. non-numerical characters"; break;
			case GML_OK: break;
		}
		cerr << endl;
		exit(1);
	}
	else
	{
		cerr << filename2 << " has " << T2.number_of_nodes() << " nodes" << endl;
	}
	
	char script_filename[256];
	strcpy (script_filename, argv[3]);
	
	
	// Get labels in trees, and set of common labels
	std::map < std::string, GTL::node, std::less<std::string> > labels_T1; 
	std::map < std::string, GTL::node, std::less<std::string> > labels_T2;
	set<string> T1_labels;
	set<string> common_labels;
	node n;
	forall_nodes (n, T1)
	{
		string s = T1.get_node_label(n);
		
		labels_T1[s] = n;
		
		T1_labels.insert(s);	
	}
	forall_nodes (n, T2)
	{
		string s = T2.get_node_label(n);	
		labels_T2[s] = n;
		
  		set<string>::iterator it = T1_labels.find(s);
  		if ( it != T1_labels.end() ) 
  		{
  			common_labels.insert(s);
  			//cout << s << endl;
  		}		
	}
	//exit(0);

	// Read edit script
	{
		ifstream f (script_filename);
		char buf[512];
		
		while (f.good())
		{
			f.getline (buf, sizeof(buf));
			
			if (f.good())
			{
				
				// Break line into tokens
				std::vector<std::string> tokens;
				std::string s = buf;
				std::string delimiters = "|";
				Tokenise (s, delimiters, tokens);
				
				if (tokens[0] == "delete")
				{
					if (tokens[1] == "node")
					{
						string name = tokens[2];
						if (labels_T1.find(name) == labels_T1.end())
						{
							cout << "Node labelled \"" << name << "\" not found" << endl;
							exit(1);
						}
						else
						{
							//cout << "Node \"" << name << "\" deleted" << endl;
							
							set<string>::iterator it = common_labels.find(name);
  							if ( it != common_labels.end() ) 
							{
								// moved
								T1.set_node_colour(labels_T1[name], "moved");
							}
							else
							{
								// deleted
								T1.set_node_colour(labels_T1[name], "deleted");
							}
							
							
							//G.del_node(l[name]);
							//l.erase(name);
						}
					}
					else if (tokens[1] == "branch")
					{
						/*
						string source = tokens[2];
						string target = tokens[3];
						if (l.find(source) == l.end())
						{
							cout << "Node labelled \"" << source << "\" not found" << endl;
							exit(1);
						}
						if (l.find(target) == l.end())
						{
							cout << "Node labelled \"" << target << "\" not found" << endl;
							exit(1);
						}
						cout << "Edge \"" << source << "\"-->\"" << target << "\" deleted" << endl;
						G.delete_edge(l[source], l[target]);
						*/
					}
				}
				if (tokens[0] == "insert")
				{
					if (tokens[1] == "node")
					{
						string name = tokens[2];
							
						set<string>::iterator it = common_labels.find(name);
						if ( it != common_labels.end() ) 
						{
							// moved
							T2.set_node_colour(labels_T2[name], "moved");
						}
						else
						{
							// inserted
							T2.set_node_colour(labels_T2[name], "inserted");
						}
					
						/*
						string name = tokens[2];
						node n = G.new_node();
						G.set_node_label (n, name);
						G.set_node_colour (n, "green");
						l[name] = n;
						cout << "Node \"" << name << "\" inserted" << endl;
						*/
					}
					else if (tokens[1] == "branch")
					{
						string source = tokens[2];
						string target = tokens[3];
						
						set<string>::iterator it = common_labels.find(target);
						if ( it != common_labels.end() ) 
						{
							// moved
							T1.set_node_colour(labels_T1[target], "moved");
							T2.set_node_colour(labels_T2[target], "moved");
						}
					
					
						/*
						string source = tokens[2];
						string target = tokens[3];
						if (l.find(source) == l.end())
						{
							cout << "Node labelled \"" << source << "\" not found" << endl;
							exit(1);
						}
						if (l.find(target) == l.end())
						{
							cout << "Node labelled \"" << target << "\" not found" << endl;
							exit(1);
						}
						
						edge e = G.new_edge (l[source], l[target]);
						G.set_edge_colour (e, "green");
						
						cout << "Edge \"" << source << "\"-->\"" << target << "\" added" << endl;
						*/
					}
				}

			}			

		}
		f.close();
	}	
	
	//exit(0);
	
	/*
	// draw tree--------------------------------------------------------------------------
	// get node labels in T1
	
	
	set<string> labels_T1;
	set<string> labels_T2;
	set<string> common_labels;
	
  	forall_nodes(v,T1) {
  		labels_T1.insert(T1.get_node_label(v));
  	}

  	forall_nodes(v,T2) {
  		string s = T2.get_node_label(v);
  		labels_T2.insert(s);
  		
  		set<string>::iterator it = labels_T1.find(s);
  		if ( it != labels_T1.end() ) 
  		{
  			common_labels.insert(s);
  		}
  	}
  	
	// set node colours
	// deleted
  	forall_nodes(v,T1) {
  		string s = T1.get_node_label(v);
  		
  		set<string>::iterator it = common_labels.find(s);
  		if ( it != common_labels.end() ) 
  		{
  			T1.set_node_colour(v, "white");
  		}
  		else
  		{
  			T1.set_node_colour(v, "#FFCC66");
  		}
  	}

	// inserted
  	forall_nodes(v,T2) {
  		string s = T2.get_node_label(v);
  		
  		set<string>::iterator it = common_labels.find(s);
  		if ( it != common_labels.end() ) 
  		{
  			T2.set_node_colour(v, "white");
  		}
  		else
  		{
  			T2.set_node_colour(v, "#80FF00");
  		}
  	}
  	*/
  	
  	// parse script....


	// draw trees--------------------------------------------------------------------------
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<style>\n";
	cout << "	html, body{\n"; 
	cout << "		width:100%; \n"; 
	cout << "		height:100%;\n"; 
	cout << "		font-size:14px;\n"; 
	cout << "	}\n"; 
	cout << "	#wrapper {\n"; 
	cout << "		padding:1%;\n"; 
	cout << "		width:96%;\n";  
	cout << "		height:90%;\n"; 
	cout << "	}\n"; 
	cout << "	.deleted {\n"; 
	cout << "		background-color:#FFCC66;\n";
	cout << "		text-decoration: line-through;\n";
	cout << "	}\n";
	cout << "	.inserted {\n";
	cout << "		background-color:#80FF00;\n";
	cout << "		font-style: italic;\n";
	cout << "	}\n";
	cout << "	.moved {\n";
	cout << "		background-color:#6CF;\n";
	cout << "	}\n";
	cout << "	.shared {\n";
	cout << "		color:#999;\n";
	cout << "	}\n";	
	cout << "	a {\n";
	cout << "		text-decoration:none;\n";
	cout << "		color:inherit;\n";
	cout << "	}\n";	
	cout << "	a hover {\n";
	cout << "		text-decoration:none;\n";
	cout << "	}\n";
	cout << "</style>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	
	cout << "<div id=\"wrapper\">" << endl;
	
    mydfs d;
    d.set_tree_number(1);

	cout << "<div style=\"float:left;width:48%;border:1px solid rgb(192,192,192);height:100%;overflow:auto;\">" << endl;

	d.start_node (T1.root());
    if (d.check(T1) != algorithm::GTL_OK) 
	{
		cerr << "dfs check failed at " << __LINE__ << " in " << __FILE__  << endl;
    } 
	else 
	{
		if (d.run(T1) != algorithm::GTL_OK) 
		{
	    	cerr << "dfs algorithm failed at " << __LINE__ << " in " << __FILE__ << endl;
		} 
	}
	
	cout << "</div>\n";
	
	
	cout << "	<div style=\"float:right;width:48%;border:1px solid rgb(192,192,192);height:100%;overflow:auto;\">\n";


	d.set_tree_number(2);
	d.start_node (T2.root());
    if (d.check(T2) != algorithm::GTL_OK) 
	{
		cerr << "dfs check failed at " << __LINE__ << " in " << __FILE__  << endl;
    } 
	else 
	{
		if (d.run(T2) != algorithm::GTL_OK) 
		{
	    	cerr << "dfs algorithm failed at " << __LINE__ << " in " << __FILE__ << endl;
		} 
	}
	
	cout << "</div>\n";
	cout << "</div>\n";
	
	cout << "<div style=\"clear:both;\"></div>\n";

	cout << "<div>\n";
	cout << "<p>Key <span class=\"deleted\">deleted</span> <span class=\"inserted\">inserted</span> <span class=\"moved\">moved</span> </p>\n";
	cout << "</div>\n";
	
	cout << "</body>\n";
	cout << "</html>\n";
	


}
