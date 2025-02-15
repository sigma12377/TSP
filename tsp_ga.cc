#include <bits/stdc++.h>
using namespace std;

template<typename T, size_t N>
double dist( const array<T, N> &p0, const array<T, N> &p1 ) {
  T d = 0;
  for( size_t i = 0 ; i < N ; ++i )
  d += ( p0[i] - p1[i] ) * ( p0[i] - p1[i] );
  return sqrt( (double)d );
}

class tsp_genome {

  double _clen = -1; // circuit length (default to a dummy value)
  vector<int> _order;

public:
  
  tsp_genome(int n): _order(n) {
    iota( begin(_order), end(_order), 0 );
    for( int i = n-1 ; i > 0 ; --i )
    swap( _order[i], _order[rand() % (i+1)] );
  }

  tsp_genome(const vector<int> &o): _order(o) {}
  tsp_genome(vector<int> &&o):      _order(o) {}

  tsp_genome(const tsp_genome &other) = default;
  tsp_genome(tsp_genome &&other)      = default;

  tsp_genome& operator=(const tsp_genome &other) = default;
  tsp_genome& operator=(tsp_genome &&other)      = default;

  const auto& get_order() const { return _order; }
  double      get_clen()  const { return _clen;  }

  template<typename T, size_t N>
  void compute_clen(const vector<array<T, N>> &pts) {
    assert( pts.size() == _order.size() );
    int n = _order.size();
    _clen = dist( pts[_order[0]], pts[_order[n-1]] );
    for( int i = 1 ; i < n ; ++i )
    _clen += dist( pts[_order[i-1]], pts[_order[i]] );
  }

  // swap two random entries in `_order`
  void mutate() {
    int n = _order.size(), a = rand() % n, b = rand() % n;
    while( b == a ) b = rand() % n;
    swap( _order[a], _order[b] );
  }
};

tsp_genome crosslink(const tsp_genome &g0, const tsp_genome &g1) {

  const auto &o0 = g0.get_order();
  vector<int> o2;
  o2.reserve( o0.size() );

  int i = rand() % (int)o0.size();
  copy_n( begin(o0), i, back_inserter(o2) );

  set<int> s( begin(o2), end(o2) );
  for( int x : g1.get_order() )
  if( s.find(x) == end(s) ) o2.push_back(x);

  return tsp_genome(o2);
}

#define GEN 10

template<typename T, size_t N>
tsp_genome shortest_path(
  const vector<array<T, N>> &pts, int pop, int gen, int keep, int mut)
{
  vector<tsp_genome> gs( pop, pts.size() );

  for( auto &g : gs ) g.compute_clen(pts);

  sort( begin(gs), end(gs), [&](const auto &g0, const auto &g1) {
    return g0.get_clen() < g1.get_clen();
  });

  printf( "Generation 0:\tshortest path is %f\n", gs[0].get_clen() );
  
  for( int i = 1 ; i <= gen ; ++i ) {
    
    for( int j = keep ; j < pop ; ++j ) {
      int a = rand() % keep, b = rand() % keep;
      while( a == b ) b = rand() % keep;
      gs[j] = crosslink( gs[a], gs[b] );
    }

    for( int j = 0 ; j < mut ; ++j )
    gs[ 1 + rand() % (pop-1) ].mutate();

    for( auto &g : gs ) g.compute_clen(pts);

    sort( begin(gs), end(gs), [&](const auto &g0, const auto &g1) {
      return g0.get_clen() < g1.get_clen();
    });

    if( 0 == i % GEN )
    printf( "Generation %d:\tshortest path is %f\n", i, gs[0].get_clen() );
  }
  return gs[0];
}

template<size_t N>
void solve( int num_pts, int pop, int gen, int keep, int mut, ifstream &file ) {

  vector<array<int, N>> pts(num_pts);
  
  for( auto &p : pts )
  for( size_t i = 0 ; i < N ; ++i ) file >> p[i];

  auto sol = shortest_path( pts, pop, gen, keep, mut );
  const auto &ord = sol.get_order();

  cout << "Best order: ";
  copy( begin(ord), end(ord), ostream_iterator<int>(cout, " ") );
  cout << "\nShortest distance: " << sol.get_clen() << '\n';
}

int main(int argc, char **argv) {
  
  srand(time(0));

  if( argc < 6 ) {
    printf( "Usage: ./a.out <filename> <pop> <gen> <keep> <mut>\n" );
    exit(1);
  }

  int num_pts, dim, pop = atoi( argv[2] ), gen = atoi( argv[3] );
  int keep = atof( argv[4] ) * pop, mut = atof( argv[5] ) * pop;

  ifstream file( argv[1] );
  file >> num_pts >> dim;

  switch(dim) {
    case 1: solve<1>(num_pts, pop, gen, keep, mut, file); break;
    case 2: solve<2>(num_pts, pop, gen, keep, mut, file); break;
    case 3: solve<3>(num_pts, pop, gen, keep, mut, file); break;
    default:
    printf("Too many dimensions\n");
  }
  
  file.close();
}
