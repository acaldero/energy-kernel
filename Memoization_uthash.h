
#ifndef MEMOIZATION_UTHASH_H
#define MEMOIZATION_UTHASH_H


  #include <stdio.h>
  #include <stdarg.h>
  #include <sys/time.h>
  #include <zlib.h>

  #include "uthash.h"

  #include <iostream>
  #include <cstdlib>
  #include <limits>
  #include <iomanip>

  #include <string>       // std::string
  #include <sstream>

  using namespace std;

	  struct MemoNode 
          {
		    float   *id;
		    long     id_size;
		    double   result_value ;
		    long     hits ;
		    bool     need_free ;
		    UT_hash_handle hh;
	  } ;

	  class MemoSupport 
          {
		private:
		    MemoNode  *memo_calls ;
		    bool       memo_isDirty ;
		    bool       memo_isEnable ;
		    char       file_name[1024] ;
		    char       component_name[1024] ;

		    void restore_bin ( void ) ;


		public:
		     MemoSupport ( string component ) ;
		    ~MemoSupport ( ) ;

		    string getComponentName() { return component_name;};

		    void    dump_bin       ( void ) ;
		    double *find           ( unsigned int bsize, float *b ) ;
		    void    add            ( unsigned int bsize, float *b, double ) ;
		    void    add_mallocated ( unsigned int bsize, float *b, double ) ;
		    void    stats          ( void ) ;
	  } ;

#endif

