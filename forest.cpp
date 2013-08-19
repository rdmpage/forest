// $Id: forest.cpp,v 1.3 2005/04/18 23:02:51 rdmp1c Exp $

#include <GTL/graph.h>
#include "mygraph.h"
#include "mytree.h"
#include "mwbmatching.h"
#include <stack>

// const MyTree& T1, const MyTree& T2
void find_common_nodes(MyTree& T1, MyTree& T2, set<string>& C) {
  node v,w;
  set<string> D;
  forall_nodes(w,T2)
    // if ( T2.is_leaf(w) )
      D.insert(T2.get_node_label(w));
  forall_nodes(v,T1) {
    // if ( T1.is_leaf(v) ) {
      string s = T1.get_node_label(v);
      set<string>::iterator it = D.find(s);
      if ( it != D.end() ) {
	C.insert(s);
      }
    // }
  }
}

void preorder_tree_list_traversal(MyTree& T, list<node>& L)
{
  L.clear();
  stack<node> S;
  S.push(T.root());
  node v,w;
  edge e;
  do {
    v = S.top();
    S.pop();
    L.push_back(v);
    forall_adj_edges(e,v) {
      w = e.target();
      S.push(w);
    }
  } while ( !S.empty() );
  // L.reverse();
}

int top_down_common_nodes(MyTree& T1, node r1, MyTree& T2, node r2,
			   node_map<set<node> >& B) {
  if ( T1.get_node_label(r1) != T2.get_node_label(r2) )
    return 0;
  if ( T1.is_leaf(r1) || T2.is_leaf(r2) )
    return T1.get_node_label(r1) == T2.get_node_label(r2);

  int p = r1.degree();
  int q = r2.degree();

  node_map<node> T1G(T1);
  node_map<node> T2G(T2);
  graph G;
  node_map<node> GT(G);
  edge_map<int> WEIGHT(G);

  node v1,v2;
  edge e1,e2;
  forall_adj_edges(e1,r1) {
    v1 = e1.target();
    node v = G.new_node();
    GT[v] = v1;
    T1G[v1] = v;
  }
  forall_adj_edges(e2,r2) {
    v2 = e2.target();
    node w = G.new_node();
    GT[w] = v2;
    T2G[v2] = w;
  }
  edge e;
  forall_adj_edges(e1,r1) {
    v1 = e1.target();
    forall_adj_edges(e2,r2) {
      v2 = e2.target();
      int res = top_down_common_nodes(T1,v1,T2,v2,B);
      // if ( res != 0 ) {
        e = G.new_edge(T1G[v1],T2G[v2]);
        WEIGHT[e] = res;
      // }
    }
  }

  list<edge> L = MAX_WEIGHT_BIPARTITE_MATCHING(G,WEIGHT);

  int res = 1; // mapping of [[r1]] to [[r2]]
  list<edge>::iterator it = L.begin();
  while ( it != L.end() ) {
    e = *it;
//    B[GT[e.source()]].insert(GT[e.target()]);
// rdmp
// Major "gotcha" here -- MAX_WEIGHT_BIPARTITE_MATCHING reverses the direction of the
// edges in G.
  B[GT[e.target()]].insert(GT[e.source()]);
    res += WEIGHT[e];
    it++;
  }
  return res;

}

void find_top_down_common_nodes(MyTree& T1, MyTree& T2, set<string>& C) {
  node_map<node> M;
  node r1 = T1.root();
  node r2 = T2.root();
  node_map<set<node> > B(T1);
  B[r1].insert(r2);
  top_down_common_nodes(T1,r1,T2,r2,B);
  M[r1] = r2;
  C.insert(T1.get_node_label(r1));
  node v,w;
  list<node> L;
  preorder_tree_list_traversal(T1,L);
  L.pop_front(); // node [[r1]] already mapped
  list<node>::iterator it = L.begin();
  while ( it != L.end() ) {
    v = *it;
    set<node>::iterator itt = B[v].begin();
    while ( itt != B[v].end() ) {
      w = *itt;
      if ( M[T1.parent(v)] == T2.parent(w) ) {
	M[v] = w;
	C.insert(T1.get_node_label(v));
	break;
      }
      itt++;
    }
    it++;
  }
}

string list_to_string(list<int>& L) {
  string S;
  list<int>::iterator it = L.begin();
  while ( it != L.end() ) {
    int i = *it;
    char ss[1024]; sprintf(ss,".%d",i);
    string s(ss);
    S += s;
    it++;
  }
  return S;
}

void find_bottom_up_common_nodes(MyTree& T1, MyTree& T2, set<string>& C) {
  node v,w;
  edge e;
  list<int> LL;

  int num = 1;
  map<string,int> CODE;

  T1.postorder_traversal(); // sets order[v] = n and number[n] = v
  int n1 = T1.number_of_nodes();
  vector<node> number1(n1+1);
  forall_nodes(v,T1)
    number1[T1.postorder(v)] = v;
  node_map<int> code1(T1);

  for (int i = 1; i <= n1; i++) {
    v = number1[i];
    string LABEL(T1.get_node_label(v));
    if ( T1.is_leaf(v) ) {
      map<string,int>::iterator it = CODE.find(LABEL);
      if ( it != CODE.end() ) {
	code1[v] = it->second;
      } else {
	CODE.insert(make_pair(LABEL,++num));
	code1[v] = num;
      }
    } else {
      LL.clear();
      forall_adj_edges(e,v)
	LL.push_back(code1[e.target()]);
      LL.sort();
      string L = list_to_string(LL);
      LABEL += L;
      map<string,int>::iterator it = CODE.find(LABEL);
      if ( it != CODE.end() ) {
	code1[v] = it->second;
      } else {
	CODE.insert(make_pair(LABEL,++num));
	code1[v] = num;
      }
    }
  }

  T2.postorder_traversal(); // sets order[w] = n and number[n] = w
  int n2 = T2.number_of_nodes();
  vector<node> number2(n2+1);
  forall_nodes(w,T2)
    number2[T2.postorder(w)] = w;
  node_map<int> code2(T2);

  for (int i = 1; i <= n2; i++) {
    w = number2[i];
    string LABEL(T2.get_node_label(w));
    if ( T2.is_leaf(w) ) {
      map<string,int>::iterator it = CODE.find(LABEL);
      if ( it != CODE.end() ) {
	code2[w] = it->second;
      } else {
	CODE.insert(make_pair(LABEL,++num));
	code2[w] = num;
      }
    } else {
      LL.clear();
      forall_adj_edges(e,w)
	LL.push_back(code2[e.target()]);
      LL.sort();
      string L = list_to_string(LL);
      LABEL += L;
      map<string,int>::iterator it = CODE.find(LABEL);
      if ( it != CODE.end() ) {
	code2[w] = it->second;
      } else {
	CODE.insert(make_pair(LABEL,++num));
	code2[w] = num;
      }
    }
  }

  set<int> SECOND;
  forall_nodes(w,T2)
    SECOND.insert(code2[w]);
  forall_nodes(v,T1) {
    set<int>::iterator it = SECOND.find(code1[v]);
    if ( it != SECOND.end() )
      C.insert(T1.get_node_label(v));
  }

}

void edit_script(MyTree& T1, MyTree& T2, set<string>& C) {
  node v, w, x, y;
  string sv, sw, sx, sy;
  edge e;

  map<string,node> L1;
  forall_nodes(v,T1)
    L1.insert(make_pair(T1.get_node_label(v),v));

  map<string,node> L2;
  forall_nodes(w,T2)
    L2.insert(make_pair(T2.get_node_label(w),w));

  int n1 = 0; // delete node count
  int n2 = 0; // insert node count
  int b1 = 0; // delete edge count
  int b2 = 0; // insert edge count

  // delete non-common nodes of T1
  forall_nodes(v,T1) {
    sv = T1.get_node_label(v);
    set<string>::iterator it = C.find(sv);
    if ( it == C.end() ) { // v is not a common node
      cout << "delete|node|" << sv << std::endl;
      n1++;
    }
  }

  // insert non-common nodes of T2
  forall_nodes(w,T2) {
    sw = T2.get_node_label(w);
    set<string>::iterator it = C.find(sw);
    if ( it == C.end() ) { // w is not a common node
      cout << "insert|node|" << sw << std::endl;
      n2++;
    }
  }

  // delete edges of T1 not in T2
  forall_nodes(v,T1) {
    sv = T1.get_node_label(v);
    set<string>::iterator it = C.find(sv);
    if ( it != C.end() ) { // v is a common node
      map<string,node>::iterator itt = L2.find(sv);
      w = itt->second;
      sw = T2.get_node_label(w);
      forall_adj_edges(e,v) {
	x = e.target();
	sx = T1.get_node_label(x);
	it = C.find(sx);
	if ( it != C.end() ) { // x is a common node
	  itt = L2.find(sx);
	  y = itt->second;
	  sy = T2.get_node_label(y);
	  if ( T2.parent(y) != w ) {
	    cout << "delete|branch|" << sv << "|" << sx << endl;
	    b1++;
	  }
	}
      }
    }
  }

  // insert edges of T2 not in T1
  forall_nodes(w,T2) {
    sw = T2.get_node_label(w);
    set<string>::iterator it = C.find(sw);
    if ( it != C.end() ) { // w is a common node
      map<string,node>::iterator itt = L1.find(sw);
      v = itt->second;
      sv = T1.get_node_label(v);
      forall_adj_edges(e,w) {
	y = e.target();
	sy = T2.get_node_label(y);
	it = C.find(sy);
	if ( it != C.end() ) { // y is a common node
	  itt = L1.find(sy);
	  x = itt->second;
	  sx = T1.get_node_label(x);
	  if ( T1.parent(x) != v ) {
	    cout << "insert|branch|" << sw << "|" << sy << endl;
	    b2++;
	  }
	}
      }
    }
  }
  
  // insert edges of T2 incident with at least one non-common node
  forall_nodes(w,T2) {
    sw = T2.get_node_label(w);
    set<string>::iterator it = C.find(sw);
    bool source = it == C.end(); // w is a non-common node
    forall_adj_edges(e,w) {
      y = e.target();
      sy = T2.get_node_label(y);
      it = C.find(sy);
      bool target = it == C.end(); // y is a non-common node
      if ( source || target ) {
	cout << "insert|branch|" << sw << "|" << sy << endl;
	b2++;
      }
    }
  }

  cerr << n1 << " of " << T1.number_of_nodes() << " nodes deleted" << endl;
  cerr << n2 << " of " << T2.number_of_nodes() << " nodes inserted" << endl;
  cerr << b1 << " of " << T1.number_of_edges() << " branches deleted" << endl;
  cerr << b2 << " of " << T2.number_of_edges() << " branches inserted" << endl;

}

int main( int argc, char *argv[] ) 
{

	if (argc < 3)
	{
		cout << "Usage: forest <file-name-1> <file-name-2>" << endl;
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

	set<string> C;
	find_common_nodes(T1,T2,C);
	// find_top_down_common_nodes(T1,T2,C);
	// find_bottom_up_common_nodes(T1,T2,C);

  // the larger the number of deletions over the number of insertions,
  // the shorter the edit script

//  if ( T1.number_of_nodes() < T2.number_of_nodes() ) {
//    edit_script(T2,T1,C);
//  } else {
    edit_script(T1,T2,C);
//  }

}

// g++ -o forest-gtl forest-gtl.cpp -lGTL
// ./forest-gtl viv_ncbi.gml viv_msw.gml
// ssh gvaliente@darwin.zoology.gla.ac.uk
