/*a Note: created by cyclicity CDL 1.3.6 - do not hand edit without adding a comment line here
 */

/*a Includes
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "be_model_includes.h"

static unsigned short sram[0x10000];

/*a Defines
 */
#define ASSIGN_TO_BIT(vector,size,bit,value) se_cmodel_assist_assign_to_bit(vector,size,bit,value)
#define ASSIGN_TO_BIT_RANGE(vector,size,bit,length,value) se_cmodel_assist_assign_to_bit_range(vector,size,bit,length,value)
#define DISPLAY_STRING(error_number,string) { \
    engine->message->add_error( NULL, error_level_info, error_number, error_id_sl_exec_file_allocate_and_read_exec_file, \
        error_arg_type_integer, engine->cycle(),\
        error_arg_type_const_string, "csram",\
        error_arg_type_malloc_string, string,\
        error_arg_type_none ); }
#define PRINT_STRING(string) { DISPLAY_STRING(error_number_se_dated_message,string); }
#define COVER_STATEMENT(stmt_number) {};
#define ASSERT_STRING(string) {}
#define ASSERT_START    { if (0 && !(
#define ASSERT_COND_NEXT       &&
#define ASSERT_COND_END        )) {
#define ASSERT_START_UNCOND { if (0) {
#define ASSERT_END      }}
#define COVER_STRING(string) {}
#define COVER_CASE_START(c,b) { if ( 0 && (
#define COVER_CASE_MESSAGE(c,b)            )) {
#define COVER_CASE_END               }}
#define COVER_START_UNCOND { if (0) {
#define COVER_END                   }}
#define WHERE_I_AM_VERBOSE {fprintf(stderr,"%s:%s:%p:%d\n",__FILE__,__func__,this,__LINE__ );}
#define WHERE_I_AM {}
#define DEFINE_DUMMY_INPUT static t_sl_uint64 dummy_input[16]={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

/*a Types for csram
*/
/*t t_csram_inputs
*/
typedef struct t_csram_inputs
{
    t_sl_uint64 *in_data;
    t_sl_uint64 *in_address;
    t_sl_uint64 *in_write_enable;
    t_sl_uint64 *in_output_enable;
} t_csram_inputs;

/*t t_csram_input_state
*/
typedef struct t_csram_input_state
{
    t_sl_uint64 in_data;
    t_sl_uint64 in_address;
    t_sl_uint64 in_write_enable;
    t_sl_uint64 in_output_enable;
} t_csram_input_state;

/*t t_csram_combinatorials
*/
typedef struct t_csram_combinatorials
{
    t_sl_uint64 out_uart;
    t_sl_uint64 out_data;
} t_csram_combinatorials;

/*t t_csram_nets
*/
typedef struct t_csram_nets
{
} t_csram_nets;

/*t c_csram
*/
class c_csram;
typedef t_sl_error_level t_csram_clock_callback_fn( void );
typedef t_csram_clock_callback_fn (c_csram::*t_c_csram_clock_callback_fn);
typedef struct
{
    t_c_csram_clock_callback_fn preclock;
    t_c_csram_clock_callback_fn clock;
} t_c_csram_clock_callback_fns;
class c_csram
{
public:
    c_csram( class c_engine *eng, void *eng_handle );
    ~c_csram();
    t_c_csram_clock_callback_fns clocks_fired[1000];
    t_sl_error_level delete_instance( void );
    t_sl_error_level reset( int pass );
    t_sl_error_level capture_inputs( void );
    t_sl_error_level propagate_inputs( void );
    t_sl_error_level propagate_state( void );
    t_sl_error_level prepreclock( void );
    t_sl_error_level preclock( t_c_csram_clock_callback_fn preclock, t_c_csram_clock_callback_fn clock );
    t_sl_error_level clock( void );
private:
    c_engine *engine;
    int clocks_to_call;
    void *engine_handle;
    int inputs_captured;
    t_csram_inputs inputs;
    t_csram_input_state input_state;
    t_csram_combinatorials combinatorials;
    t_csram_nets nets;
    struct t_engine_log_event_array *log_event_array;
    t_se_signal_value *log_signal_data;
};

/*a Static variables for csram
*/
/*v struct_offset wrapper
*/
#define struct_offset( ptr, a ) (((char *)&(ptr->a))-(char *)ptr)
/*v state_desc_csram_combs
*/
static t_csram_combinatorials *___csram_comb__ptr;
static t_engine_state_desc state_desc_csram_combs[] =
{
    {"out_uart",engine_state_desc_type_bits, NULL, struct_offset(___csram_comb__ptr, out_uart), {16,0,0,0}, {NULL,NULL,NULL,NULL} },
    {"out_data",engine_state_desc_type_bits, NULL, struct_offset(___csram_comb__ptr, out_data), {16,0,0,0}, {NULL,NULL,NULL,NULL} },
    {"", engine_state_desc_type_none, NULL, 0, {0,0,0,0}, {NULL,NULL,NULL,NULL} }
};

/*v struct_offset unwrapper
*/
#undef struct_offset

/*a Static wrapper functions for csram
*/
/*f csram_instance_fn
*/
static t_sl_error_level csram_instance_fn( c_engine *engine, void *engine_handle )
{
    c_csram *mod;
    mod = new c_csram( engine, engine_handle );
    if (!mod)
        return error_level_fatal;
    return error_level_okay;
}

/*f csram_delete_fn - simple callback wrapper for the main method
*/
static t_sl_error_level csram_delete_fn( void *handle )
{
    c_csram *mod;
    t_sl_error_level result;
    mod = (c_csram *)handle;
    result = mod->delete_instance();
    delete( mod );
    return result;
}

/*f csram_reset_fn
*/
static t_sl_error_level csram_reset_fn( void *handle, int pass )
{
    c_csram *mod;
    mod = (c_csram *)handle;
    return mod->reset( pass );
}

/*f csram_combinatorial_fn
*/
static t_sl_error_level csram_combinatorial_fn( void *handle )
{
    c_csram *mod;
    mod = (c_csram *)handle;
    mod->capture_inputs();
    return mod->propagate_inputs();
}

/*a Constructors and destructors for csram
*/
/*f c_csram::c_csram
*/
c_csram::c_csram( class c_engine *eng, void *eng_handle )
{
    engine = eng;
    engine_handle = eng_handle;

    engine->register_delete_function( engine_handle, (void *)this, csram_delete_fn );
    engine->register_reset_function( engine_handle, (void *)this, csram_reset_fn );

    memset(&inputs, 0, sizeof(inputs));
    memset(&input_state, 0, sizeof(input_state));
    memset(&combinatorials, 0, sizeof(combinatorials));
    memset(&nets, 0, sizeof(nets));

    engine->register_comb_fn( engine_handle, (void *)this, csram_combinatorial_fn );
    engine->register_propagate_fn( engine_handle, (void *)this, csram_combinatorial_fn );

    engine->register_input_signal( engine_handle, "in_data", 16, &inputs.in_data );
    engine->register_comb_input( engine_handle, "in_data" );
    engine->register_input_signal( engine_handle, "in_address", 16, &inputs.in_address );
    engine->register_comb_input( engine_handle, "in_address" );
    engine->register_input_signal( engine_handle, "in_write_enable", 1, &inputs.in_write_enable );
    engine->register_comb_input( engine_handle, "in_write_enable" );
    engine->register_input_signal( engine_handle, "in_output_enable", 1, &inputs.in_output_enable );
    engine->register_comb_input( engine_handle, "in_output_enable" );

    engine->register_output_signal( engine_handle, "out_uart", 16, &combinatorials.out_uart );
    engine->register_comb_output( engine_handle, "out_uart" );
    combinatorials.out_uart = 0;
    engine->register_output_signal( engine_handle, "out_data", 16, &combinatorials.out_data );
    engine->register_comb_output( engine_handle, "out_data" );
    combinatorials.out_data = 0;



    engine->register_state_desc( engine_handle, 1, state_desc_csram_combs, &combinatorials, NULL );
}

/*f c_csram::~c_csram
*/
c_csram::~c_csram()
{
    delete_instance();
}

/*f c_csram::delete_instance
*/
t_sl_error_level c_csram::delete_instance( void )
{
    return error_level_okay;
}

/*a Class reset/preclock/clock methods for csram
*/
/*f c_csram::reset
*/
t_sl_error_level c_csram::reset( int pass )
{
    if (pass==0)
        {
        int unconnected_inputs; unconnected_inputs=0;
        int unconnected_nets; unconnected_nets=0;
        if (!inputs.in_data) {DEFINE_DUMMY_INPUT;fprintf(stderr,"POSSIBLY FATAL:Unconnected input in_data on module csram at initial reset\n");unconnected_inputs=1;inputs.in_data=&dummy_input[0];}
        if (!inputs.in_address) {DEFINE_DUMMY_INPUT;fprintf(stderr,"POSSIBLY FATAL:Unconnected input in_address on module csram at initial reset\n");unconnected_inputs=1;inputs.in_address=&dummy_input[0];}
        if (!inputs.in_write_enable) {DEFINE_DUMMY_INPUT;fprintf(stderr,"POSSIBLY FATAL:Unconnected input in_write_enable on module csram at initial reset\n");unconnected_inputs=1;inputs.in_write_enable=&dummy_input[0];}
        if (!inputs.in_output_enable) {DEFINE_DUMMY_INPUT;fprintf(stderr,"POSSIBLY FATAL:Unconnected input in_output_enable on module csram at initial reset\n");unconnected_inputs=1;inputs.in_output_enable=&dummy_input[0];}
        }
    if (pass>0) {capture_inputs(); propagate_inputs(); propagate_state();} // Dont call capture_inputs on first pass as they may be invalid; wait for second pass
    return error_level_okay;
}

/*f c_csram::capture_inputs
*/
t_sl_error_level c_csram::capture_inputs( void )
{
    input_state.in_data = inputs.in_data[0];
    input_state.in_address = inputs.in_address[0];
    input_state.in_write_enable = inputs.in_write_enable[0];
    input_state.in_output_enable = inputs.in_output_enable[0];
    return error_level_okay;
}
/*f c_csram::propagate_inputs
*/
t_sl_error_level c_csram::propagate_inputs( void )
{

    combinatorials.out_uart = 0x0000;
    combinatorials.out_data = 0xFFFF;

    if ((input_state.in_output_enable)==(0x1LL))
    {
        combinatorials.out_data = sram[input_state.in_address];
    }
    if ((input_state.in_write_enable)==(0x1LL))
    {
        if(input_state.in_address == 0xF000)
        {
            combinatorials.out_uart = input_state.in_data;
            //printf("uart_out 0x%02X\n",(unsigned short)(input_state.in_data&0xFFFF));
        }
        else
        {
            sram[input_state.in_address] = input_state.in_data;
        }
    }
    return error_level_okay;
}

/*f c_csram::propagate_state
*/
t_sl_error_level c_csram::propagate_state( void )
{
    return propagate_inputs();
}
/*a Initialization functions
*/
/*f csram__init
*/
extern void csram__init( void )
{

#include "sram.h"

    se_external_module_register( 1, "csram", csram_instance_fn );
}

/*a Scripting support code
*/
/*f initcsram
*/
extern "C" void initcsram( void )
{
    csram__init( );
    scripting_init_module( "csram" );
}
