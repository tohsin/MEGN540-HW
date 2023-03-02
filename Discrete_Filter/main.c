/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/

#include "Filter.h"

#include <math.h>  // max
#include <stdio.h>

int main()
{
    int running_score = 0;
    int total_score   = 0;

    float a_coeff[] = { 0, 1, 2, 3 };
    float b_coeff[] = { 4, 5, 6, 7 };

    int filter_order = sizeof( a_coeff ) / sizeof( float ) - 1;

    // printf("Filter order is: %li\n", sizeof(a_coeff)/sizeof(float) - 1 );

    Filter_Data_t moving_average;
    Filter_Init( &moving_average, b_coeff, a_coeff, sizeof( a_coeff ) / sizeof( float ) - 1 );

    total_score++;
    if( rb_length_F( &moving_average.denominator ) == filter_order + 1 ) {
        running_score++;
    } else {
        printf( "Initialization Error: denominator initialized to the wrong size. is: %i, should be %i.\n", rb_length_F( &moving_average.denominator ),
                filter_order + 1 );
    }

    total_score++;
    if( rb_length_F( &moving_average.numerator ) == filter_order + 1 ) {
        running_score++;
    } else {
        printf( "Initialization Error: numerator initialized to the wrong size. is: %i, should be %i.\n", rb_length_F( &moving_average.numerator ),
                filter_order + 1 );
    }

    total_score++;
    if( rb_length_F( &moving_average.in_list ) == filter_order + 1 ) {
        running_score++;
    } else {
        printf( "Initialization Error: in_list initialized to the wrong size. is: %i, should be %i.\n", rb_length_F( &moving_average.in_list ),
                filter_order + 1 );
    }

    total_score++;
    if( rb_length_F( &moving_average.out_list ) == filter_order + 1 ) {
        running_score++;
    } else {
        printf( "Initialization Error: out_list initialized to the wrong size. is: %i, should be %i.\n", rb_length_F( &moving_average.out_list ),
                filter_order + 1 );
    }

    if( running_score != total_score ) {
        printf( "\n---------\n\nInitialization sizes incorrect. Skipping content and functional checks.\n\n\n Score 0.\n" );
        return 0;
    }

    // Begin Content Checks
    for( int i = 0; i <= filter_order; i++ ) {
        total_score++;
        if( fabs( rb_get_F( &moving_average.denominator, i ) - a_coeff[i] ) < 1e-5 ) {
            running_score++;
        } else {
            printf( "Initialization Error: denominator index %i incorrect. is: %f, exp: %f\n", i, rb_get_F( &moving_average.denominator, i ), a_coeff[i] );
        }

        total_score++;
        if( fabs( rb_get_F( &moving_average.numerator, i ) - b_coeff[i] ) < 1e-5 ) {
            running_score++;
        } else {
            printf( "Initialization Error: numerator index %i incorrect. is: %f, exp: %f\n", i, rb_get_F( &moving_average.numerator, i ), b_coeff[i] );
        }

        total_score++;
        if( rb_get_F( &moving_average.in_list, i ) == 0 ) {
            running_score++;
        } else {
            printf( "Initialization Error: in_list index %i not initialized to zero.\n", i );
        }

        total_score++;
        if( rb_get_F( &moving_average.out_list, i ) == 0 ) {
            running_score++;
        } else {
            printf( "Initialization Error: out_list index %i not initialized to zero.\n", i );
        }
    }

    if( running_score != total_score ) {
        printf( "\n---------\n\nInitialization Incorrect. Skipping functional tests.\n\n\n" );
        printf( "Score is 0.\n" );
        return 0;
    }

    // Shift_Filter
    Filter_ShiftBy( &moving_average, 1.23 );
    for( int i = 0; i <= filter_order; i++ ) {
        total_score++;
        if( fabs( rb_get_F( &moving_average.in_list, i ) - 1.23 ) < 1e-5 ) {
            running_score++;
        } else {
            printf( "Filter_ShiftBy Error: in_list index %i not shifted correctly.\n", i );
        }

        total_score++;
        if( fabs( rb_get_F( &moving_average.out_list, i ) - 1.23 ) < 1e-5 ) {
            running_score++;
        } else {
            printf( "Filter_ShiftBy Error: out_list  index %i not shifted correctly.\n", i );
        }
    }

    Filter_SetTo( &moving_average, 2.35 );
    for( int i = 0; i <= filter_order; i++ ) {
        total_score++;
        if( fabs( rb_get_F( &moving_average.in_list, i ) - 2.35 ) < 1e-5 ) {
            running_score++;
        } else {
            printf( "Filter_SetTo Error: in_list index %i not set correctly.\n", i );
        }

        total_score++;
        if( fabs( rb_get_F( &moving_average.out_list, i ) - 2.35 ) < 1e-5 ) {
            running_score++;
        } else {
            printf( "Filter_SetTo Error: out_list  index %i not set correctly.\n", i );
        }
    }

    float num[]  = { 1, 1, 1, 1, 1 };
    float den[]  = { 5, 0, 0, 0, 0 };
    filter_order = sizeof( num ) / sizeof( float ) - 1;
    Filter_Init( &moving_average, num, den, filter_order );

    for( int i = 0; i < RB_LENGTH_F * 2; i++ ) {

        float avg = ( i - 5 ) * ( i > 4 ) + ( i > 4 ? 3 : i * ( i + 1 ) / 10. );

        float filt_avg = Filter_Value( &moving_average, i );

        total_score++;
        if( fabs( filt_avg - avg ) < 1e-4 ) {
            running_score++;
        } else {
            printf( "Error in filtering on Filter_Value call # %i for Filter order: %i.\t is: %f \tShould be: %f\n", i, filter_order, filt_avg, avg );
        }
    }

    float num2[] = { 0.046582906636443696668514746761502, 0.18633162654577478667405898704601, 0.2794974398186621522555128649401,
                     0.18633162654577478667405898704601, 0.046582906636443696668514746761502 };
    float den2[] = { 1.0, -0.78209519802333749005640584073262, 0.67997852691629945276474700222025, -0.18267569775303207912919845057331,
                     0.030118875043169249239305429455271 };
    filter_order = sizeof( num ) / sizeof( float ) - 1;
    Filter_Init( &moving_average, num2, den2, filter_order );
    float filter_out = Filter_Value( &moving_average, 3.5 );
    float filter_exp = 0.1630401760;
    total_score++;
    if( fabs( filter_out - filter_exp ) < 1e-5 ) {
        running_score++;
    } else {
        printf( "Error in filtering on Filter_Value call for real filter.\t is: %f \texp:%f\n", filter_out, filter_exp );
    }

    filter_out = Filter_Value( &moving_average, 2.3 );
    filter_exp = 0.8868142962;
    total_score++;
    if( fabs( filter_out - filter_exp ) < 1e-5 ) {
        running_score++;
    } else {
        printf( "Error in filtering on Filter_Value call for real filter.\t is: %f \texp:%f\n", filter_out, filter_exp );
    }

    filter_out = Filter_Value( &moving_average, -1.4 );
    filter_exp = 1.9242972136;
    total_score++;
    if( fabs( filter_out - filter_exp ) < 1e-5 ) {
        running_score++;
    } else {
        printf( "Error in filtering on Filter_Value call for real filter.\t is: %f \texp:%f\n", filter_out, filter_exp );
    }

    filter_out = Filter_Value( &moving_average, 6.9 );
    filter_exp = 2.2873148918;
    total_score++;
    if( fabs( filter_out - filter_exp ) < 1e-5 ) {
        running_score++;
    } else {
        printf( "Error in filtering on Filter_Value call for real filter.\t is: %f \texp:%f\n", filter_out, filter_exp );
    }

    filter_out = Filter_Value( &moving_average, -2 );
    filter_exp = 2.0303349495;
    total_score++;
    if( fabs( filter_out - filter_exp ) < 1e-5 ) {
        running_score++;
    } else {
        printf( "Error in filtering on Filter_Value call for real filter.\t is: %f \texp:%f\n", filter_out, filter_exp );
    }

    filter_out = Filter_Value( &moving_average, -3 );
    filter_exp = 1.6197994947;
    total_score++;
    if( fabs( filter_out - filter_exp ) < 1e-5 ) {
        running_score++;
    } else {
        printf( "Error in filtering on Filter_Value call for real filter.\t is: %f \texp:%f\n", filter_out, filter_exp );
    }

    float filt_last = Filter_Value( &moving_average, 0 );
    total_score += 10;
    if( fabs( filt_last - Filter_Last_Output( &moving_average ) ) < 1e-4 ) {
        running_score += 10;
    } else {
        printf( "Error in Filter_Last_Output wrong value returned.\n" );
    }

    printf( "Testing Done!\n\tScore %i / %i: %2.2f%%\n", running_score, total_score, 100.0 * running_score / total_score );

    return 0;
}
