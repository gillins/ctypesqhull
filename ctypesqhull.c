
#include <stdio.h>
#include <libqhull_r/qhull_ra.h>

_Static_assert(sizeof(realT) == sizeof(double), "qhull must be compiled with realT == double");

// #define DUMP_ARRAY

double ctypesqhull(double *points, int numpoints, int dim) 
{
    qhT qhstruct;
    char flags[25];
    sprintf (flags, "qhull s FA");
    
#ifdef DUMP_ARRAY
    int idx = 0;
    for( int p = 0; p < numpoints; p++)
    {
        printf("point %d\n", p);
        for( int d = 0; d < dim; d++)
        {
            printf("%f ", points[idx]);
            idx++;
        }
        printf("\n");
    }
#endif

    qh_zero(&qhstruct, stderr);
    qh_new_qhull(&qhstruct, dim, numpoints, points, 0, flags, NULL, NULL);
    qh_getarea(&qhstruct, qhstruct.facet_list);
    double result = qhstruct.totvol;
    qh_freeqhull(&qhstruct, qh_ALL);
    int curlong, totlong;
    qh_memfreeshort(&qhstruct, &curlong, &totlong);

    return result;
}
