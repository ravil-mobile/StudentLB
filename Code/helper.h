#ifndef __HELPER_H__
#define __HELPER_H__

/* includefiles */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include <string>
#include <vector>
#include <list>

#include "LBDefinitions.h"
#include "DataStructure.h"

/* ----------------------------------------------------------------------- */
/*                      user define functions                              */
/* ----------------------------------------------------------------------- */
inline unsigned computeFlagIndex( unsigned x,
							 	  unsigned y,
							 	  unsigned z,
							 	  unsigned * Length ) {

	int TotalLengthX = Length[ 0 ] + 2;
	int TotalLengthY = Length[ 1 ] + 2;

    return ( z * TotalLengthX * TotalLengthY ) + ( y * TotalLengthX ) + x;
}

// function to calculate lexicographical co-ordinates of the lattices in the field
inline unsigned computeFieldIndex( unsigned x,
							  	   unsigned y,
							  	   unsigned z,
							  	   unsigned * Length ) {

	int TotalLengthX = Length[ 0 ] + 2;
	int TotalLengthY = Length[ 1 ] + 2;

    return Vel_DOF * ( ( z * TotalLengthX * TotalLengthY ) + ( y * TotalLengthX ) + x );
}

inline unsigned convertFieldToFlagIndex( unsigned FieldIndex ) {
	return ( unsigned ) ( FieldIndex / Vel_DOF );
}


void allocateFields( double** collideField,
					 double** streamField,
				   	 int** flagField,
				  	 int** IdField,
				  	 unsigned* Length );

void getLengthFromString( unsigned* Length, std::string String );
void copyToVector( std::list<Fluid*> &aList, std::vector<Fluid*> &aVector );


#ifdef PI
#undef PI
#endif

#define FREE_ARG char*

/**
 * Maximum length of input lines
 */
#define MAX_LINE_LENGTH 1024

/**
 * Stores the last timer value
 */
extern clock_t last_timer_reset;


int read_parameters( const char *INPUT_FILE_NAME,        /* the name of the data file */
                     double *Tau,                        /* relaxation time */
                     unsigned *TimeSteps,                /* number of simulation time steps */
                     unsigned *TimeStepsPerPlotting,     /* number of visualization time steps */
					 int RANK );


void errhandler( int nLine,
                 const char *szFile,
                 const char *szString );


char* find_string( const char* szFileName,
                   const char *szVarName );


void read_string( const char* szFileName,
                  const char* szVarName,
                  char*   pVariable);


void read_int( const char* szFileName,
               const char* szVarName,
               int* pVariable,
			   int RANK );


void read_unsigned( const char* szFileName,
			   		const char* szVarName,
			   		unsigned* pVariable);


void read_double( const char* szFileName,
                  const char* szVarName,
                  double* pVariable,
				  int RANK );


int min_int( const int n1, const int n2 );




// DEBUGGING: this functions overlaps with the function declared in the std library
//int min( int a, int b);
//int max( int a, int b);
//double fmin( double a, double b);
//double fmax( double a, double b);

template< class T >
T min( T a, T b );

template< class T >
T max( T a, T b );


/**
 * Error handling:
 *
 * ERROR(s) writes an error message and terminates the program
 *
 * Example:
 * ERROR("File not found !");
 */
#define ERROR(s)    errhandler( __LINE__, __FILE__, s)

/**
 * Error handling:
 *
 * ERROR(s) writes an error message and terminates the program
 *
 * Example:
 * ERROR("File not found !");
 */
#define ERROUT stdout

/**
 * Error handling:
 *
 * ERROR(s) writes an error message and terminates the program
 *
 * Example:
 * ERROR("File not found !");
 */
void  errhandler( int nLine, const char *szFile, const char *szString );


/**
 * Reading from a datafile.
 *
 * The foloowing three macros help reading values from the parameter file.
 * If a variable cannot be found, the program stops with an error message.
 *
 * Example:
 * READ_INT( "MyFile.dat", imax );
 * READ_STRING( szFile, szProblem );
 */
#define READ_INT( szFileName, VarName)    read_int   ( szFileName, #VarName, &(VarName) )

/**
 * Reading from a datafile.
 *
 * The foloowing three macros help reading values from the parameter file.
 * If a variable cannot be found, the program stops with an error message.
 *
 * Example:
 * READ_INT( "MyFile.dat", imax );
 * READ_STRING( szFile, szProblem );
 */
#define READ_DOUBLE( szFileName, VarName) read_double( szFileName, #VarName, &(VarName) )

/**
 * Reading from a datafile.
 *
 * The foloowing three macros help reading values from the parameter file.
 * If a variable cannot be found, the program stops with an error message.
 *
 * Example:
 * READ_INT( "MyFile.dat", imax );
 * READ_STRING( szFile, szProblem );
 */
#define READ_STRING( szFileName, VarName) read_string( szFileName, #VarName,  (VarName) )

void read_string( const char* szFilename, const char* szName, char*  sValue);
void read_int   ( const char* szFilename, const char* szName, int*    nValue);
void read_double( const char* szFilename, const char* szName, double*  Value);


/**
 * Writing matrices to a file.
 * -----------------------------------------------------------------------
 * write_matrix(...) wites a matrice to a file
 * the file has the following format
 *
 *    -----------------------------------------
 *    |  Length          |  float  |  ASCII  |
 *    ----------------------------------------|
 *    |  ylength          |  float  |  ASCII  |
 *    ----------------------------------------|
 *    |  nrl              |  int    |  ASCII  |
 *    ----------------------------------------|
 *    |  nrh              |  int    |  ASCII  |    1. call of the
 *    ----------------------------------------|
 *    |  ncl              |  int    |  ASCII  |
 *    ----------------------------------------|
 *    |  nch              |  int    |  ASCII  |    1. call of the
 *    ----------------------------------------|    function with
 *    |  m[nrl][ncl]      |  float  |  binaer |    bFirst == 1
 *    ----------------------------------------|
 *    |  m[nrl][ncl+1]    |  float  |  binaer |
 *    ----------------------------------------|
 *    |                  .                    |
 *                       .
 *    |                  .                    |
 *    -----------------------------------------
 *    |  m[nrh][nch]      |  float  |  binary |
 *    -----------------------------------------------------------------
 *    |  m[nrl][ncl]      |  float  |  binary |
 *    ----------------------------------------|
 *    |  m[nrl][ncl+1]    |  float  |  binary |     2. call with
 *    ----------------------------------------|     bFirst == 0
 *    |                  .                    |
 *                       .
 *    |                  .                    |
 *    -----------------------------------------
 *    |  m[nrh][nch]      |  float  |  binary |
 *    ------------------------------------------------------------------
 *
 * @param szFileName          name of the file
 * @param m                   matrix
 * @param nrl                 first column
 * @param nrh                 last column
 * @param ncl                 first row
 * @param nch                 last row
 * @param Length             size of the geometry in x-direction
 * @param ylength             size of the geometry in y-direction
 * @param Length             size of the geometry in x-direction
 * @param fFirst              0 == append, else overwrite
 */
void write_matrix(
  const char* szFileName,
  double **m,
  int nrl,
  int nrh,
  int ncl,
  int nch,
  double Length,
  double ylength,
  int fFirst
);

/**
 * @param szFileName    filehandle
 * @param m             matrix
 * @param nrl           first column
 * @param nrh           last column
 * @param ncl           first row
 * @param nch           last row
 */
void read_matrix( const char* szFileName,      /* filehandle */
                  double **m,                  /* matrix */
                  int nrl,                     /* first column */
                  int nrh,                     /* last column */
                  int ncl,                     /* first row */
                  int nch );                   /* last row */


/**
 * matrix(...)        storage allocation for a matrix (nrl..nrh, ncl..nch)
 * free_matrix(...)   storage deallocation
 * init_matrix(...)   initialization of all matrix entries with a fixed
 *                  (floating point) value
 * imatrix(...)       analog for matrices with integer-entries
 *
 * Example:
 *    U = matrix ( 0 , imax+1 , 0 , jmax+1 );
 *    init_matrix( U , 0, imax+1, 0, jmax+1, 0 );
 *    free_matrix( U,  0, imax+1, 0, jmax+1 );
 */
double **matrix( int nrl, int nrh, int ncl, int nch );
/**
 * matrix(...)        storage allocation for a matrix (nrl..nrh, ncl..nch)
 * free_matrix(...)   storage deallocation
 * init_matrix(...)   initialization of all matrix entries with a fixed
 *                  (floating point) value
 * imatrix(...)       analog for matrices with integer-entries
 *
 * Example:
 *    U = matrix ( 0 , imax+1 , 0 , jmax+1 );
 *    init_matrix( U , 0, imax+1, 0, jmax+1, 0 );
 *    free_matrix( U,  0, imax+1, 0, jmax+1 );
 */
void free_matrix( double **m, int nrl, int nrh, int ncl, int nch );
/**
 * matrix(...)        storage allocation for a matrix (nrl..nrh, ncl..nch)
 * free_matrix(...)   storage deallocation
 * init_matrix(...)   initialization of all matrix entries with a fixed
 *                  (floating point) value
 * imatrix(...)       analog for matrices with integer-entries
 *
 * Example:
 *    U = matrix ( 0 , imax+1 , 0 , jmax+1 );
 *    init_matrix( U , 0, imax+1, 0, jmax+1, 0 );
 *    free_matrix( U,  0, imax+1, 0, jmax+1 );
 */
void init_matrix( double **m, int nrl, int nrh, int ncl, int nch, double a);

/**
 * matrix(...)        storage allocation for a matrix (nrl..nrh, ncl..nch)
 * free_matrix(...)   storage deallocation
 * init_matrix(...)   initialization of all matrix entries with a fixed
 *                  (floating point) value
 * imatrix(...)       analog for matrices with integer-entries
 *
 * Example:
 *    U = matrix ( 0 , imax+1 , 0 , jmax+1 );
 *    init_matrix( U , 0, imax+1, 0, jmax+1, 0 );
 *    free_matrix( U,  0, imax+1, 0, jmax+1 );
 */
int  **imatrix( int nrl, int nrh, int ncl, int nch );
/**
 * matrix(...)        storage allocation for a matrix (nrl..nrh, ncl..nch)
 * free_matrix(...)   storage deallocation
 * init_matrix(...)   initialization of all matrix entries with a fixed
 *                  (floating point) value
 * imatrix(...)       analog for matrices with integer-entries
 *
 * Example:
 *    U = matrix ( 0 , imax+1 , 0 , jmax+1 );
 *    init_matrix( U , 0, imax+1, 0, jmax+1, 0 );
 *    free_matrix( U,  0, imax+1, 0, jmax+1 );
 */
void free_imatrix( int **m, int nrl, int nrh, int ncl, int nch );
/**
 * matrix(...)        storage allocation for a matrix (nrl..nrh, ncl..nch)
 * free_matrix(...)   storage deallocation
 * init_matrix(...)   initialization of all matrix entries with a fixed
 *                  (floating point) value
 * imatrix(...)       analog for matrices with integer-entries
 *
 * Example:
 *    U = matrix ( 0 , imax+1 , 0 , jmax+1 );
 *    init_matrix( U , 0, imax+1, 0, jmax+1, 0 );
 *    free_matrix( U,  0, imax+1, 0, jmax+1 );
 */
void init_imatrix( int **m, int nrl, int nrh, int ncl, int nch, int a);


/**
 * reads in a ASCII pgm-file and returns the colour information in a two-dimensional integer array.
 * At this, a boundary layer around the image is additionally stored and initialised with 0.
 */
int **read_pgm(const char *filename);


/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMPOUT stdout

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_POSITION() fprintf( DUMPOUT, "%s:%d Dumpposition \n", __FILE__, __LINE__ )

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_MESSAGE(s) fprintf( DUMPOUT, "%s:%d %s\n",            __FILE__, __LINE__, s  )

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_INT(n)     fprintf( DUMPOUT, "%s:%d %s = %d\n", __FILE__, __LINE__, #n, n )

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_DOUBLE(d)  fprintf( DUMPOUT, "%s:%d %s = %f\n", __FILE__, __LINE__, #d, d )

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_STRING(s)  fprintf( DUMPOUT, "%s:%d %s = %s\n", __FILE__, __LINE__, #s, s )

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define RESET_TIMER()   last_timer_reset = clock()

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_TIMER()    fprintf( DUMPOUT, "%s:%d Timer: %f\n", __FILE__, __LINE__, (float)(clock()-last_timer_reset)/(float)CLOCKS_PER_SEC )

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_MATRIX_TO_FILE( m, nrl, nrh, ncl, nch, Length, ylength) \
        {  \
           static nCount = 0; \
           char szFileName[100];  \
           sprintf( szFileName, "%s__%d__%s.out", __FILE__, __LINE__, #m); \
           write_matrix( szFileName, m, nrl, nrh, ncl, nch, Length, ylength, nCount == 0); \
           ++nCount; \
        }

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_INT_TO_FILE(n) \
        {  \
           static nCount = 0; \
           FILE *fh = 0; \
           char szFileName[100];  \
           sprintf( szFileName, "%s__%d__%s.out", __FILE__, __LINE__, #n); \
           if( nCount == 0) \
              fh = fopen( szFileName, "w"); \
           else  \
              fh = fopen( szFileName, "a"); \
           if( fh )  \
              fprintf( fh, "%d:%d\n", nCount, n ); \
           else  \
              ERROR("Fehler beim Dumpen");  \
           fclose(fh);  \
           ++nCount; \
        }

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_DOUBLE_TO_FILE(d) \
        {  \
           static nCount = 0; \
           FILE *fh = 0; \
           char szFileName[100];  \
           sprintf( szFileName, "%s__%d__%s.out", __FILE__, __LINE__, #d); \
           if( nCount == 0) \
              fh = fopen( szFileName, "w"); \
           else  \
              fh = fopen( szFileName, "a"); \
           if( fh )  \
              fprintf( fh, "%d:%f\n", nCount, d ); \
           else  \
              ERROR("Fehler beim Dumpen");  \
           fclose(fh);  \
           ++nCount; \
        }

/**
 *                         useful macros
 * -----------------------------------------------------------------------
 *  The following macros can be helpful to display variables during the
 *  runtime of the program.
 *  If you start the program in a shell from xemacs, you can jump to the
 *  respectove rows by switching to the compilation-minor-mode.
 *
 *  DUMP_POSITION()           dumps the actual position within the program
 *  DUMP_MESSAGE( .)          dump a message in addition
 *  DUMP_INT(..)              dump an integer variable
 *
 *  DUMP_MATRIX_TO_FILE(..)
 *  DUMP_INT_TO_FILE(..)      writes the value of the variable in
 *  DUMP_DOUBLE_TO_FILE(..)   a tracefile
 *  DUMP_STRING_TO_FILE(..)
 *
 *  RESET_TIMER()     set timer to zero
 *  DUMP_TIMER()      dump time that has passed since the last
 *                    RESET_TIMER()
 */
#define DUMP_STRING_TO_FILE(s) \
        {  \
           static nCount = 0; \
           FILE *fh = 0; \
           char szFileName[100];  \
           sprintf( szFileName, "%s__%d__%s.out", __FILE__, __LINE__, #s); \
           if( nCount == 0) \
              fh = fopen( szFileName, "w"); \
           else  \
              fh = fopen( szFileName, "a"); \
           if( fh )  \
              fprintf( fh, "%d:%s\n", nCount, s ); \
           else  \
              ERROR("Fehler beim Dumpen");  \
           fclose(fh);  \
           ++nCount; \
        }

#endif
