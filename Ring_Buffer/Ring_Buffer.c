/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2023.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2023 Andrew Petruska at Colorado School of Mines

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

#include "Ring_Buffer.h"

#include <stdio.h>  // required for the printf in rb_print_data_X functions


// define constant masks for use later based on length chosen
// static makes these global scope only to this c file
static const uint8_t RB_MASK_F = RB_LENGTH_F - 1;
static const uint8_t RB_MASK_B = RB_LENGTH_B - 1;


// Helper Functions start here

void increment_start_index_F(Ring_Buffer_Float_t* p_buf){
    p_buf->start_index = (p_buf->start_index + 1) & RB_MASK_F;
}
void increment_start_index_B(Ring_Buffer_Byte_t* p_buf){
    p_buf->start_index = (p_buf->start_index + 1) & RB_MASK_B;
}

void increment_end_index_F(Ring_Buffer_Float_t* p_buf){
    p_buf->end_index = (p_buf->end_index + 1) & RB_MASK_F;
}

void increment_end_index_B(Ring_Buffer_Byte_t* p_buf){
    p_buf->end_index = (p_buf->end_index + 1) & RB_MASK_B;
}

void decrement_start_index_F(Ring_Buffer_Float_t* p_buf){
    p_buf->start_index = (p_buf->start_index - 1) & RB_MASK_F;
}

void decrement_start_index_B(Ring_Buffer_Byte_t* p_buf){
    p_buf->start_index = (p_buf->start_index - 1) & RB_MASK_B;
}

void decrement_end_index_F(Ring_Buffer_Float_t* p_buf){
    p_buf->end_index = (p_buf->end_index - 1) & RB_MASK_F;
}

void decrement_end_index_B(Ring_Buffer_Byte_t* p_buf){
    p_buf->end_index = (p_buf->end_index - 1) & RB_MASK_B;
}

uint8_t start_index_equals_end_index_F(Ring_Buffer_Float_t* p_buf){
    return p_buf->start_index == p_buf->end_index;
}

uint8_t start_index_equals_end_index_B(Ring_Buffer_Byte_t* p_buf){
    return p_buf->start_index == p_buf->end_index;
}

// helper functions end here

/* Initialization */
void rb_initialize_F( Ring_Buffer_Float_t* p_buf )
{
    // set start and end indices to 0
    // no point changing data
    p_buf->start_index = 0;
    p_buf->end_index   = 0;
}

void rb_initialize_B( Ring_Buffer_Byte_t* p_buf )
{
    // set start and end indices to 0
    // no point changing data
    p_buf->start_index = 0;
    p_buf->end_index   = 0;
}

/* Return active Length of Buffer */
uint8_t rb_length_F( const Ring_Buffer_Float_t* p_buf )
{
    // calculate the active length using the mask and 2's complement to help
    // verify for your self why this works!
    uint8_t length = ( p_buf->end_index - p_buf->start_index ) & RB_MASK_F;
    return length;
}
uint8_t rb_length_B( const Ring_Buffer_Byte_t* p_buf )
{
    // your code here!
    // make sure to use the correct mask!
    uint8_t length = ( p_buf->end_index - p_buf->start_index ) & RB_MASK_B;
    return length;
}

/* Append element to end and lengthen */
void rb_push_back_F( Ring_Buffer_Float_t* p_buf, float value )
{
    // Put data at index end
    // Increment the end index and wrap using the mask.
    // If the end equals the start increment the start index
    p_buf->buffer[p_buf->end_index] = value;

    increment_end_index_F(p_buf);
    // p_buf->end_index  = ( p_buf->end_index + 1 ) & RB_MASK_F;
    if( start_index_equals_end_index_F(p_buf) )
        increment_start_index_F(p_buf);
    // your code here!
}
void rb_push_back_B( Ring_Buffer_Byte_t* p_buf, uint8_t value )
{
    // Put data at index end
    // Increment the end index and wrap using the mask.
    // If the end equals the start increment the start index
    p_buf->buffer[p_buf->end_index] = value;
    increment_end_index_B(p_buf);
    if( start_index_equals_end_index_B(p_buf) )
        increment_start_index_B(p_buf);
    // your code here!
}

/* Append element to front and lengthen */
void rb_push_front_F( Ring_Buffer_Float_t* p_buf, float value )
{
    // Decrement the start index and wrap using the mask.
    // If the end equals the start decrement the end index
    // Set the value at the start index as desired.
    decrement_start_index_F(p_buf);
    if( start_index_equals_end_index_F(p_buf)  )
        decrement_end_index_F(p_buf);
    p_buf->buffer[p_buf->start_index] = value;
    // your code here!
}
void rb_push_front_B( Ring_Buffer_Byte_t* p_buf, uint8_t value )
{
    // Decrement the start index and wrap using the mask.
    // If the end equals the start decrement the end index
    // Set the value at the start index as desired.
    decrement_start_index_B(p_buf);
    if(start_index_equals_end_index_B(p_buf) )
        decrement_end_index_B(p_buf);
    p_buf->buffer[p_buf->start_index] = value;
    // your code here!
}

/* Remove element from end and shorten */
float rb_pop_back_F( Ring_Buffer_Float_t* p_buf )
{
    // if end does not equal start (length zero),
    //    reduce end index by 1 and mask
    // 	  return value at at end
    // else return zero if your lis is length zero
    
    if(!(start_index_equals_end_index_F(p_buf)))
    {
        decrement_end_index_F(p_buf);
        return p_buf->buffer[p_buf->end_index];
    }
    // your code here!
    return 0;
}
uint8_t rb_pop_back_B( Ring_Buffer_Byte_t* p_buf )
{
    // if end does not equal start (length zero),
    //    reduce end index by 1 and mask
    // 	  return value at at end
    // else return zero if list is length zero
    if(! start_index_equals_end_index_B(p_buf))
    {
        decrement_end_index_B(p_buf);
        return p_buf->buffer[p_buf->end_index];
    }
    // your code here!
    return 0;
}

/* Remove element from start and shorten */
float rb_pop_front_F( Ring_Buffer_Float_t* p_buf )
{
    // if end does not equal start (length zero),
    //    get value to return at front
    //    increase start index by 1 and mask
    //    return value
    // else return zero if length of list is zero
    if(! start_index_equals_end_index_F(p_buf))
    {
        float value = p_buf->buffer[p_buf->start_index];
        increment_start_index_F(p_buf);
        return value;
    }

    // your code here!
    return 0;
}
uint8_t rb_pop_front_B( Ring_Buffer_Byte_t* p_buf )
{
    // if end does not equal start (length zero),
    //    get value to return at front
    //    increase start index by 1 and mask
    //    return value
    // else return zero if length of list is zero
    if(! start_index_equals_end_index_B(p_buf))
    {
        uint8_t value = p_buf->buffer[p_buf->start_index];
        increment_start_index_B(p_buf);
        return value;
    }
    // your code here!
    return 0;
}

/* access element */
float rb_get_F( const Ring_Buffer_Float_t* p_buf, uint8_t index )
{
    // return value at start + index wrapped properly
    return p_buf->buffer[(p_buf->start_index + index) & RB_MASK_F];
    // your code here!
}
uint8_t rb_get_B( const Ring_Buffer_Byte_t* p_buf, uint8_t index )
{
    // return value at start + index wrapped properly
    return p_buf->buffer[(p_buf->start_index + index) & RB_MASK_B];
}

/* set element - This behavior is
   poorly defined if inedex is outside of active length.
   Use of the push_back or push_front methods are prefered.
*/
void rb_set_F( Ring_Buffer_Float_t* p_buf, uint8_t index, float value )
{
    // set value at start + index wrapped properly
    p_buf->buffer[(p_buf->start_index + index) & RB_MASK_F] = value;
}
void rb_set_B( Ring_Buffer_Byte_t* p_buf, uint8_t index, uint8_t value )
{
    // set value at start + index wrapped properly
    p_buf->buffer[(p_buf->start_index + index) & RB_MASK_B] = value;
}

#ifndef AVR_MCU  // dont build these for the car builds
/*
 * The below functions are provided to help you debug. They print out the length, start and end index, active elements,
 * and the contents of the buffer.
 */
void rb_print_data_F( Ring_Buffer_Float_t* p_buf )
{
    printf( "-------FLOAT RINGBUFFER INFO--------\nRing Buffer Length: %i\nStart index: %i\nEnd index: %i\n", rb_length_F( p_buf ), p_buf->start_index,
            p_buf->end_index );

    printf( "\nActive Storage\n" );
    for( int i = 0; i < rb_length_F( p_buf ); i++ )
        printf( "Index: %i, Internal Index: %i, Value: %f\n", i, p_buf->start_index + i, rb_get_F( p_buf, i ) );

    printf( "\nInternal Storage\n" );
    for( int i = 0; i < RB_LENGTH_F; i++ )
        printf( "Internal Index: %i, Value: %f\n", i, p_buf->buffer[i] );

    printf( "-------END FLOAT RINGBUFFER INFO---------\n\n" );
}

void rb_print_data_B( Ring_Buffer_Byte_t* p_buf )
{
    printf( "-------BYTE RINGBUFFER INFO--------\nRing Buffer Length: %i\nStart index: %i\nEnd index: %i\n", rb_length_B( p_buf ), p_buf->start_index,
            p_buf->end_index );

    printf( "\nActive Storage\n" );
    for( int i = 0; i < rb_length_B( p_buf ); i++ )
        printf( "Index: %i, Internal Index: %i, Value (ascii): %c, Value (hex): 0x%X\n", i, p_buf->start_index + i, rb_get_B( p_buf, i ),
                rb_get_B( p_buf, i ) );

    printf( "\nInternal Storage\n" );
    for( int i = 0; i < RB_LENGTH_B; i++ )
        printf( "Internal Index: %i, Value (ascii): %c, Value (hex): 0x%X\n", i, p_buf->buffer[i], p_buf->buffer[i] );

    printf( "-------END BYTE RINGBUFFER INFO---------\n\n" );
}
#endif