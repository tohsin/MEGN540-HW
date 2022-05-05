#include "yaml-cpp/yaml.h"

#include "gtest/gtest.h"

#include <iostream>
#include <vector>

extern "C" {
#include "../solution/Filter.h"
#include "../solution/Ring_Buffer.h"
}

static float near_tol = 1e-5;

void expeq( Ring_Buffer_F* p_rb, std::vector< float > vec, const char* fun_name, const char* array_name )
{
    for( uint8_t i = 0; i < rb_length_F( p_rb ); i++ )
        EXPECT_NEAR( vec[i], rb_get_F( p_rb, i ), near_tol ) << "Function: " << fun_name << " Ring Buffer: " << array_name << " at index " << (int)i << "\n";
}
void init( Filter_Data_t* p_filt )
{
    YAML::Node node              = YAML::LoadFile( "filter_solution.yaml" );
    std::vector< float > num_vec = node["numerator"].as< std::vector< float > >();
    std::vector< float > den_vec = node["denominator"].as< std::vector< float > >();

    Filter_Init( p_filt, &num_vec[0], &den_vec[0], num_vec.size() - 1 );

    Filter_SetTo( p_filt, 0 );
}

TEST( Filter, Filter_Init )
{
    YAML::Node node = YAML::LoadFile( "filter_solution.yaml" );

    Filter_Data_t filter;
    init( &filter );

    std::vector< float > num_vec = node["Filter_Init.numerator"].as< std::vector< float > >();
    std::vector< float > den_vec = node["Filter_Init.denominator"].as< std::vector< float > >();
    std::vector< float > in_vec  = node["Filter_Init.in_list"].as< std::vector< float > >();
    std::vector< float > out_vec = node["Filter_Init.out_list"].as< std::vector< float > >();

    expeq( &filter.numerator, num_vec, "Filter_Init", "numerator" );
    expeq( &filter.denominator, den_vec, "Filter_Init", "denominator" );
    expeq( &filter.in_list, in_vec, "Filter_Init", "in_list" );
    expeq( &filter.out_list, out_vec, "Filter_Init", "out_list" );
}

TEST( Filter, Filter_SetTo )
{
    YAML::Node node = YAML::LoadFile( "filter_solution.yaml" );

    Filter_Data_t filter;
    init( &filter );

    std::vector< float > num_vec = node["Filter_SetTo.numerator"].as< std::vector< float > >();
    std::vector< float > den_vec = node["Filter_SetTo.denominator"].as< std::vector< float > >();
    std::vector< float > in_vec  = node["Filter_SetTo.in_list"].as< std::vector< float > >();
    std::vector< float > out_vec = node["Filter_SetTo.out_list"].as< std::vector< float > >();
    float val                    = node["Filter_SetTo.val"].as< float >();

    Filter_SetTo( &filter, val );

    expeq( &filter.numerator, num_vec, "Filter_SetTo", "numerator" );
    expeq( &filter.denominator, den_vec, "Filter_SetTo", "denominator" );
    expeq( &filter.in_list, in_vec, "Filter_SetTo", "in_vec" );
    expeq( &filter.out_list, out_vec, "Filter_SetTo", "out_vec" );
}

TEST( Filter, Filter_ShiftBy )
{
    YAML::Node node = YAML::LoadFile( "filter_solution.yaml" );

    Filter_Data_t filter;
    init( &filter );

    std::vector< float > num_vec = node["Filter_ShiftBy.numerator"].as< std::vector< float > >();
    std::vector< float > den_vec = node["Filter_ShiftBy.denominator"].as< std::vector< float > >();
    std::vector< float > in_vec  = node["Filter_ShiftBy.in_list"].as< std::vector< float > >();
    std::vector< float > out_vec = node["Filter_ShiftBy.out_list"].as< std::vector< float > >();
    float val                    = node["Filter_ShiftBy.val"].as< float >();

    Filter_SetTo( &filter, val );

    expeq( &filter.numerator, num_vec, "Filter_ShiftBy", "numerator" );
    expeq( &filter.denominator, den_vec, "Filter_ShiftBy", "denominator" );
    expeq( &filter.in_list, in_vec, "Filter_ShiftBy", "in_list" );
    expeq( &filter.out_list, out_vec, "Filter_ShiftBy", "out_list" );
}

TEST( Filter, Filter_Value )
{
    YAML::Node node = YAML::LoadFile( "filter_solution.yaml" );

    Filter_Data_t filter;
    init( &filter );

    std::vector< float > num_vec = node["Filter_Value.numerator"].as< std::vector< float > >();
    std::vector< float > den_vec = node["Filter_Value.denominator"].as< std::vector< float > >();
    std::vector< float > in_vec  = node["Filter_Value.in_list"].as< std::vector< float > >();
    std::vector< float > out_vec = node["Filter_Value.out_list"].as< std::vector< float > >();
    float val                    = node["Filter_Value.val"].as< float >();
    float expected_val           = node["Filter_Value.ret"].as< float >();

    float calculated_val = Filter_Value( &filter, val );
    EXPECT_FLOAT_EQ( expected_val, calculated_val );

    expeq( &filter.numerator, num_vec, "Filter_Value First Update", "numerator" );
    expeq( &filter.denominator, den_vec, "Filter_Value First Update", "denominator" );
    expeq( &filter.in_list, in_vec, "Filter_Value First Update", "in_list" );
    expeq( &filter.out_list, out_vec, "Filter_Value First Update", "out_list" );
}

TEST( Filter, Filter_Value_Step )
{
    YAML::Node node = YAML::LoadFile( "filter_solution.yaml" );

    Filter_Data_t filter;
    init( &filter );

    std::vector< float > num_vec      = node["Filter_Value_Step.numerator"].as< std::vector< float > >();
    std::vector< float > den_vec      = node["Filter_Value_Step.denominator"].as< std::vector< float > >();
    std::vector< float > in_vec       = node["Filter_Value_Step.in_list"].as< std::vector< float > >();
    std::vector< float > out_vec      = node["Filter_Value_Step.out_list"].as< std::vector< float > >();
    float val                         = node["Filter_Value_Step.val"].as< float >();
    std::vector< float > expected_val = node["Filter_Value_Step.ret"].as< std::vector< float > >();

    for( int i = 0; i < expected_val.size(); i++ ) {
        float calculated_val = Filter_Value( &filter, val );
        EXPECT_NEAR( expected_val[i], calculated_val, near_tol ) << "Filter_Value_Step iteration: " << (int)i;
    }

    expeq( &filter.numerator, num_vec, "Filter_Value Response After Last Update", "numerator" );
    expeq( &filter.denominator, den_vec, "Filter_Value Response After Last Update", "denominator" );
    expeq( &filter.in_list, in_vec, "Filter_Value Response After Last Update", "in_list" );
    expeq( &filter.out_list, out_vec, "Filter_Value Response After Last Update", "out_list" );
}

TEST( Filter, Filter_Last_Output )
{
    YAML::Node node = YAML::LoadFile( "filter_solution.yaml" );

    Filter_Data_t filter;
    init( &filter );

    std::vector< float > num_vec      = node["Filter_Value_Step.numerator"].as< std::vector< float > >();
    std::vector< float > den_vec      = node["Filter_Value_Step.denominator"].as< std::vector< float > >();
    std::vector< float > in_vec       = node["Filter_Value_Step.in_list"].as< std::vector< float > >();
    std::vector< float > out_vec      = node["Filter_Value_Step.out_list"].as< std::vector< float > >();
    float val                         = node["Filter_Value_Step.val"].as< float >();
    std::vector< float > expected_val = node["Filter_Value_Step.ret"].as< std::vector< float > >();

    for( int i = 0; i < expected_val.size(); i++ ) {
        Filter_Value( &filter, val );
        float calculated_val = Filter_Last_Output( &filter );
        EXPECT_NEAR( expected_val[i], calculated_val, near_tol ) << "Filter_Value_Step iteration: " << (int)i;
    }

    expeq( &filter.numerator, num_vec, "Filter_Value Step Response After Last Update", "numerator" );
    expeq( &filter.denominator, den_vec, "Filter_Value_Step Response After Last Update", "denominator" );
    expeq( &filter.in_list, in_vec, "Filter_Value_Step Response After Last Update", "in_list" );
    expeq( &filter.out_list, out_vec, "Filter_Value_Step Response After Last Update", "out_list" );
}

int main( int argc, char** argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}