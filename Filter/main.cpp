extern "C" {
#include "solution/Filter.h"
#include "solution/Ring_Buffer.h"
}

#include "yaml-cpp/yaml.h"

#include <fstream>
#include <vector>

std::vector< float > rb_to_vec( Ring_Buffer_F* p_rb )
{
    std::vector< float > ret;
    for( uint8_t i = 0; i < rb_length_F( p_rb ); i++ )
        ret.push_back( rb_get_F( p_rb, i ) );

    return std::move( ret );
}

int main()
{
    YAML::Node node;  // starts out as null

    node["key"] = "value";                     // it now is a map node
    node["seq"].push_back( "first element" );  // node["seq"] automatically becomes a sequence
    node["seq"].push_back( "second element" );

    float num[] = { 0.004824343358, 0.019297373431, 0.028946060146, 0.019297373431, 0.004824343358 };
    float den[] = { 1.000000000000, -2.369513007182, 2.313988414416, -1.054665405879, 0.187379492368 };

    std::vector< float > num_vec( num, num + sizeof( num ) / sizeof( num[0] ) );
    std::vector< float > den_vec( den, den + sizeof( den ) / sizeof( den[0] ) );

    node["numerator"]   = num_vec;
    node["denominator"] = den_vec;
    std::vector< std::string > keys;
    std::string key_id;
    float val;

    {
        Filter_Data_t filter;
        Filter_Init( &filter, num, den, sizeof( num ) / sizeof( num[0] ) - 1 );
        key_id = "Filter_Init";
        keys.push_back( key_id );

        node[key_id + ".numerator"]   = rb_to_vec( &filter.numerator );
        node[key_id + ".denominator"] = rb_to_vec( &filter.denominator );
        node[key_id + ".in_list"]     = rb_to_vec( &filter.in_list );
        node[key_id + ".out_list"]    = rb_to_vec( &filter.out_list );
    }

    {
        Filter_Data_t filter;
        Filter_Init( &filter, num, den, sizeof( num ) / sizeof( num[0] ) - 1 );

        val = 1.1;
        Filter_SetTo( &filter, val );
        key_id = "Filter_SetTo";
        keys.push_back( key_id );
        node[key_id + ".val"]         = val;
        node[key_id + ".numerator"]   = rb_to_vec( &filter.numerator );
        node[key_id + ".denominator"] = rb_to_vec( &filter.denominator );
        node[key_id + ".in_list"]     = rb_to_vec( &filter.in_list );
        node[key_id + ".out_list"]    = rb_to_vec( &filter.out_list );
    }

    {
        Filter_Data_t filter;
        Filter_Init( &filter, num, den, sizeof( num ) / sizeof( num[0] ) - 1 );

        val = -1.1;
        Filter_ShiftBy( &filter, val );
        key_id = "Filter_ShiftBy";
        keys.push_back( key_id );
        node[key_id + ".val"]         = val;
        node[key_id + ".numerator"]   = rb_to_vec( &filter.numerator );
        node[key_id + ".denominator"] = rb_to_vec( &filter.denominator );
        node[key_id + ".in_list"]     = rb_to_vec( &filter.in_list );
        node[key_id + ".out_list"]    = rb_to_vec( &filter.out_list );
    }

    {
        Filter_Data_t filter;
        Filter_Init( &filter, num, den, sizeof( num ) / sizeof( num[0] ) - 1 );

        val       = -1.1;
        float ret = Filter_Value( &filter, val );
        key_id    = "Filter_Value";
        keys.push_back( key_id );
        node[key_id + ".val"]         = val;
        node[key_id + ".ret"]         = ret;
        node[key_id + ".numerator"]   = rb_to_vec( &filter.numerator );
        node[key_id + ".denominator"] = rb_to_vec( &filter.denominator );
        node[key_id + ".in_list"]     = rb_to_vec( &filter.in_list );
        node[key_id + ".out_list"]    = rb_to_vec( &filter.out_list );
    }

    {
        Filter_Data_t filter;
        Filter_Init( &filter, num, den, sizeof( num ) / sizeof( num[0] ) - 1 );

        val    = 1;
        key_id = "Filter_Value_Step";
        keys.push_back( key_id );
        std::vector< float > ret;
        for( int i = 0; i < 10; i++ )
            ret.push_back( Filter_Value( &filter, val ) );

        node[key_id + ".val"]         = val;
        node[key_id + ".ret"]         = ret;
        node[key_id + ".numerator"]   = rb_to_vec( &filter.numerator );
        node[key_id + ".denominator"] = rb_to_vec( &filter.denominator );
        node[key_id + ".in_list"]     = rb_to_vec( &filter.in_list );
        node[key_id + ".out_list"]    = rb_to_vec( &filter.out_list );
    }

    {
        Filter_Data_t filter;
        Filter_Init( &filter, num, den, sizeof( num ) / sizeof( num[0] ) - 1 );

        val    = 1;
        key_id = "Filter_Last_Output";
        keys.push_back( key_id );
        std::vector< float > ret;
        for( int i = 0; i < 10; i++ )
            ret.push_back( Filter_Value( &filter, val ) );

        node[key_id + ".val"]         = val;
        node[key_id + ".ret"]         = ret;
        node[key_id + ".numerator"]   = rb_to_vec( &filter.numerator );
        node[key_id + ".denominator"] = rb_to_vec( &filter.denominator );
        node[key_id + ".in_list"]     = rb_to_vec( &filter.in_list );
        node[key_id + ".out_list"]    = rb_to_vec( &filter.out_list );
    }

    node["Test"] = keys;

    std::ofstream fout( "filter_solution.yaml" );
    YAML::Emitter out;
    out << YAML::Precision( 9 ) << node;
    fout << out.c_str();

    return 0;
}
