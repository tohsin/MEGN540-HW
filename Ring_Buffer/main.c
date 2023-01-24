#include <stdio.h>
#include <math.h>

#include "Ring_Buffer.h"



int main(void)
{

    // Example Use Case
    struct Ring_Buffer_F rbF_pt;
    rb_initialize_F(&rbF_pt);

    rb_push_back_F(&rbF_pt,1);
    rb_push_back_F(&rbF_pt,2);
    rb_push_back_F(&rbF_pt,3);
    rb_push_back_F(&rbF_pt,4);
    rb_push_back_F(&rbF_pt,5);
    rb_pop_front_F(&rbF_pt);
    rb_pop_front_F(&rbF_pt);

    rb_print_data_F(&rbF_pt); // should be length 3 printing 3,4,5 as the active values


    struct Ring_Buffer_B rbC_pt;
    rb_initialize_B(&rbC_pt);

    rb_push_back_B(&rbC_pt,'a');
    rb_push_back_B(&rbC_pt,'b');
    rb_push_back_B(&rbC_pt,'c');
    rb_push_back_B(&rbC_pt,'d');
    rb_push_back_B(&rbC_pt,'e');
    rb_pop_front_B(&rbC_pt);
    rb_pop_front_B(&rbC_pt);

    rb_print_data_B(&rbC_pt); // should be length 3 printing c,d,e as the active values


    // Evaluation Portion of the script

    int score = 0;
    //needed for testing
    const uint8_t RB_MASK_F = RB_LENGTH_F-1;
    const uint8_t RB_MASK_B = RB_LENGTH_B-1;
    float float_tol = .001;

//Init (F)
    rb_initialize_F(&rbF_pt);
    if (rbF_pt.start_index != 0)
        printf("rb_initialize_F: start_index = %i, should = 0. \n",rbF_pt.start_index);
    else
        score++;

    if (rbF_pt.end_index != 0)
        printf("rb_initialize_F: end_index = %i, should = 0. \n",rbF_pt.end_index);
    else
        score++;


    //init (C)
    rb_initialize_B(&rbC_pt);
    if (rbC_pt.start_index != 0)
        printf("rb_initialize_B: start_index = %i, should = 0. \n",rbF_pt.start_index);
    else
        score++;

    if (rbC_pt.end_index != 0)
        printf("rb_initialize_B: end_index = %i, should = 0. \n",rbF_pt.end_index);
    else
        score++;



//length (F)
    //normal forward case
    rbF_pt.start_index=(RB_LENGTH_F-3);
    rbF_pt.end_index=(RB_LENGTH_F-1);
    int F_len = rb_length_F(&rbF_pt);
    if (F_len != 2)
        printf("rb_length_F: normal case: = %i, should = 2. \n",F_len); //2
    else
        score++;

    //Wrap case
    rbF_pt.end_index=1;
    F_len = rb_length_F(&rbF_pt);
    if (F_len != ((rbF_pt.end_index-rbF_pt.start_index)&RB_MASK_F))
        printf("rb_length_F: wrap case: = %i, should = %i. \n",F_len,(rbF_pt.end_index-rbF_pt.start_index)&RB_MASK_F);
    else
        score++;


    //length (C)
    //normal forward case
    rbC_pt.start_index=(RB_LENGTH_B-3);
    rbC_pt.end_index=(RB_LENGTH_B-1);
    int C_len = rb_length_B(&rbC_pt);
    if (C_len != 2)
        printf("rb_length_B: normal case: = %i, should = 2. \n",C_len);
    else
        score++;

    //Wrap case
    rbC_pt.end_index=1;
    C_len = rb_length_B(&rbC_pt);
    if (C_len != ((rbC_pt.end_index-rbC_pt.start_index)&RB_MASK_B))
        printf("rb_length_B: wrap case: = %i, should = %i. \n",C_len,(rbC_pt.end_index-rbC_pt.start_index)&RB_MASK_B);
    else
        score++;



//push_back (F)
    //check end wrap, start increment, and set buffer value
    rbF_pt.start_index=0;
    rbF_pt.end_index=(RB_LENGTH_F-1);

    rb_push_back_F(&rbF_pt,17.1);
    if (rbF_pt.end_index != 0)
        printf("rb_push_back_F: end_index = %i, should = 0 . \n",rbF_pt.end_index);
    else
        score++;

    if (rbF_pt.start_index != 1)
        printf("rb_push_back_F: start_index = %i, should = 1. \n",rbF_pt.start_index);
    else
        score++;

    float val_F = rbF_pt.buffer[ (rbF_pt.end_index-1) & RB_MASK_F];
    if (fabs(val_F- 17.1) > float_tol ) //probably a  better way to do this...
        printf("rb_push_back_F: value, should = 17.1: %f \n",val_F);
    else
        score++;


    //push_back (C)
    //check end wrap, start increment, and set buffer value
    rbC_pt.start_index=0;
    rbC_pt.end_index=(RB_LENGTH_B-1);

    rb_push_back_B(&rbC_pt,'d');
    if (rbC_pt.end_index != 0)
        printf("rb_push_back_B: end_index = %i, should = 0 . \n",rbC_pt.end_index);
    else
        score++;

    if (rbC_pt.start_index != 1)
        printf("rb_push_back_B: start_index = %i, should = 1. \n",rbC_pt.start_index);
    else
        score++;

    char val_B = rbC_pt.buffer[ (rbC_pt.end_index-1) & RB_MASK_B];
    if (val_B != 'd')
        printf("rb_push_back_B: value, should = 'd': %c \n",val_B);
    else
        score++;



//push_front (F)
    //check start wrap, end increment, and set buffer value
    rbF_pt.start_index=0;
    rbF_pt.end_index=(RB_LENGTH_F-1);

    rb_push_front_F(&rbF_pt,18.2);
    if (rbF_pt.end_index != (RB_LENGTH_F-2))
        printf("rb_push_front_F: end_index = %i, should = %i . \n",rbF_pt.end_index,(RB_LENGTH_F-2));
    else
        score++;

    if (rbF_pt.start_index != (RB_LENGTH_F-1))
        printf("rb_push_front_F: start_index = %i, should = %i. \n",rbF_pt.start_index,(RB_LENGTH_F-1));
    else
        score++;

    val_F = rbF_pt.buffer[rbF_pt.start_index & RB_MASK_F];
    if (fabs(val_F- 18.2) > float_tol ) //probably a  better way to do this...
        printf("rb_push_front_F: value, should = 18.2: %f \n",val_F);
    else
        score++;


    //push_front (C)
    //check start wrap, end increment, and set buffer value
    rbC_pt.start_index=0;
    rbC_pt.end_index=(RB_LENGTH_B-1);

    rb_push_front_B(&rbC_pt,'e');
    if (rbC_pt.end_index != (RB_LENGTH_B-2))
        printf("rb_push_front_B: end_index = %i, should = %i . \n",rbC_pt.end_index,(RB_LENGTH_B-2));
    else
        score++;

    if (rbC_pt.start_index != (RB_LENGTH_B-1))
        printf("rb_push_front_B: start_index = %i, should = %i. \n",rbC_pt.start_index,(RB_LENGTH_B-1));
    else
        score++;

    val_B = rbC_pt.buffer[rbC_pt.start_index & RB_MASK_B];
    if (val_B != 'e')
        printf("rb_push_front_B: value, should = 'e': %c \n",val_B);
    else
        score++;


//pop_back (F)
    //check end wrap, end increment, and buffer value
    rbF_pt.buffer[(RB_LENGTH_F-1)]=5.3;
    rbF_pt.start_index=(RB_LENGTH_F-1);
    rbF_pt.end_index=0;

    val_F = rb_pop_back_F(&rbF_pt);

    if (rbF_pt.end_index != (RB_LENGTH_F-1))
        printf("rb_pop_back_F: end_index = %i, should = %i . \n",rbF_pt.end_index,(RB_LENGTH_F-1));
    else
        score++;

    if (fabs(val_F- 5.3) > float_tol ) //probably a  better way to do this...
        printf("rb_pop_back_F: value, should = 5.3: %f \n",val_F);
    else
        score++;


    //check end=start case
    val_F = rb_pop_back_F(&rbF_pt);

    if (rbF_pt.start_index != (RB_LENGTH_F-1))
        printf("rb_pop_back_F: start_index = %i, should = %i . \n",rbF_pt.end_index,(RB_LENGTH_F-1));


    //pop_back (C)
    //check end wrap, end increment, and buffer value
    rbC_pt.buffer[(RB_LENGTH_B-1)]='h';
    rbC_pt.start_index=(RB_LENGTH_B-1);
    rbC_pt.end_index=0;

    val_B = rb_pop_back_B(&rbC_pt);

    if (rbC_pt.end_index != (RB_LENGTH_B-1))
        printf("rb_pop_back_B: end_index = %i, should = %i . \n",rbC_pt.end_index,(RB_LENGTH_B-1));
    else
        score++;

    if (val_B != 'h')
        printf("rb_pop_back_B: value, should = 'h': %c \n",val_B);
    else
        score++;

    //check end=start case
    val_B = rb_pop_back_B(&rbC_pt);

    if (rbC_pt.start_index != (RB_LENGTH_B-1))
        printf("rb_pop_back_B: start_index = %i, should = %i . \n",rbC_pt.end_index,(RB_LENGTH_B-1));
    else
        score++;



//pop_front (F)
    //check end wrap, end increment, and buffer value
    rbF_pt.buffer[RB_LENGTH_F-1]=5.3;
    rbF_pt.start_index=RB_LENGTH_F-1;
    rbF_pt.end_index=0;

    val_F = rb_pop_front_F(&rbF_pt);

    if (rbF_pt.start_index != 0)
        printf("rb_pop_front_F: start_index = %i, should = 0 . \n",rbF_pt.start_index);
    else
        score++;

    if (fabs(val_F- 5.3) > float_tol ) //probably a  better way to do this...
        printf("rb_pop_front_F: value, should = 5.3: %f \n",val_F);
    else
        score++;


    //check end=start case
    val_F = rb_pop_front_F(&rbF_pt);
    if (rbF_pt.start_index != 0)
        printf("rb_pop_front_F: start_index = %i, should = 0 . \n",rbF_pt.start_index);
    else
        score++;


    //pop_front (C)
    //check end wrap, end increment, and buffer value
    rbC_pt.buffer[RB_LENGTH_B-1]='h';
    rbC_pt.start_index=RB_LENGTH_B-1;
    rbC_pt.end_index=0;

    val_B = rb_pop_front_B(&rbC_pt);

    if (rbC_pt.start_index != 0)
        printf("rb_pop_front_B: start_index = %i, should = 0 . \n",rbC_pt.start_index);
    else
        score++;

    if (val_B != 'h')
        printf("rb_pop_front_B: is %c, should be: 'h' \n",val_B);
    else
        score++;

    //check end=start case
    val_B = rb_pop_front_B(&rbC_pt);

    if (rbC_pt.start_index != 0)
        printf("rb_pop_front_B: start_index = %i, should = 0 . \n",rbC_pt.start_index);
    else
        score++;



//access element (F)
    //check wrap and val
    rbF_pt.buffer[4]=9.8;
    rbF_pt.end_index = RB_LENGTH_F-2;
    rbF_pt.start_index=RB_LENGTH_F-1;
    val_F = rb_get_F(&rbF_pt,5);
    if (fabs(val_F- 9.8) > float_tol ) //probably a  better way to do this...
        printf("rb_get_F: is %f, should be: = 9.8 \n",val_F);
    else
        score++;


//access element (C)
    rbC_pt.buffer[3]='e';
    rbC_pt.buffer[4]='j';
    rbC_pt.buffer[5]='l';
    rbC_pt.end_index = RB_LENGTH_B-2;
    rbC_pt.start_index = RB_LENGTH_B-1;
    val_B = rb_get_B(&rbC_pt,5);
    if (val_B != 'j')
        printf("rb_get_B: value is %c, should be = 'j' \n",val_B);
    else
        score++;



//set element (F)
    //check wrap and val
    rbF_pt.start_index=RB_LENGTH_F-1;
    rb_set_F(&rbF_pt,3,21.4);
    val_F = rbF_pt.buffer[2];
    if (fabs(val_F- 21.4) > float_tol ) //probably a  better way to do this...
        printf("rb_set_F: value is %f, should be: 21.4:\n",val_F);
    else
        score++;


//set element (C)
    //check wrap and val
    rbC_pt.start_index=RB_LENGTH_B-1;
    rb_set_B(&rbC_pt,3,'k');
    val_B = rbC_pt.buffer[2];
    if (val_B != 'k')
        printf("rb_set_B: value is %c, should be 'k' \n",val_B);
    else
        score++;



    printf("Score: %2.1f out of 75\n",(float)score/35.0*75);

}

