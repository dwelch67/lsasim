//a Note: created by CDL 1.3.6 - do not hand edit without recognizing it will be out of sync with the source
// Output mode 0 (VMOD=1, standard verilog=0)

//a Module lsa_core
module lsa_core
(
    in_clock,

    in_reset

);

    //b Clocks
    input in_clock;

    //b Inputs
    input in_reset;

    //b Outputs

// output components here

    //b Output combinatorials

    //b Output nets

    //b Internal and output registers
    reg [7:0]leds;
    reg [15:0]inst;
    reg [3:0]astate;
    reg [15:0]reg__value[63:0];

    //b Internal combinatorials
    reg mem_fetch;
    reg mem_rd;
    reg mem_wr;
    reg [15:0]mem_in;
    reg [15:0]mem_out;
    reg [15:0]mem_add;
    reg [16:0]op_res;
    reg [16:0]op_b;
    reg [16:0]op_a;
    reg [3:0]astate_next;
    reg [15:0]pc_next;

    //b Internal nets

    //b Clock gating module instances
    //b Module instances
    //b the_mem__comb combinatorial process
    always @( //the_mem__comb
        mem_rd or
        mem_add )
    begin: the_mem__comb_code
    reg [15:0]mem_out__var;
        mem_out__var = 16'h0;
        if ((mem_rd==1'h1))
        begin
            case (mem_add) //synopsys parallel_case
            16'h0: // req 1
                begin
                mem_out__var = 16'hc001;
                end
            16'h1: // req 1
                begin
                mem_out__var = 16'hc000;
                end
            16'h2: // req 1
                begin
                mem_out__var = 16'h97f1;
                end
            16'h3: // req 1
                begin
                mem_out__var = 16'h8500;
                end
            16'h4: // req 1
                begin
                mem_out__var = 16'h8820;
                end
            16'h5: // req 1
                begin
                mem_out__var = 16'h6889;
                end
            16'h6: // req 1
                begin
                mem_out__var = 16'hc2fe;
                end
            16'h7: // req 1
                begin
                mem_out__var = 16'h6558;
                end
            16'h8: // req 1
                begin
                mem_out__var = 16'h4758;
                end
            16'h9: // req 1
                begin
                mem_out__var = 16'h8820;
                end
            16'ha: // req 1
                begin
                mem_out__var = 16'hc0fa;
                end
            default: // req 1
                begin
                mem_out__var = 16'h0;
                end
            endcase
        end //if
        mem_out = mem_out__var;
    end //always

    //b the_mem__posedge_in_clock_active_low_in_reset clock process
    always @( posedge in_clock or negedge in_reset)
    begin : the_mem__posedge_in_clock_active_low_in_reset__code
        if (in_reset==1'b0)
        begin
            leds <= 8'h0;
        end
        else
        begin
            if ((mem_wr==1'h1))
            begin
                case (mem_add) //synopsys parallel_case
                16'hf100: // req 1
                    begin
                    leds <= mem_in[7:0];
                    end
                default: // req 1
                    begin
                    end
                endcase
            end //if
        end //if
    end //always

    //b the_code__comb combinatorial process
    always @( //the_code__comb
        astate or
        reg__value[0] or
        reg__value[1] or
        reg__value[2] or
        reg__value[3] or
        reg__value[4] or
        reg__value[5] or
        reg__value[6] or
        reg__value[7] or
        reg__value[8] or
        reg__value[9] or
        reg__value[10] or
        reg__value[11] or
        reg__value[12] or
        reg__value[13] or
        reg__value[14] or
        reg__value[15] or
        reg__value[16] or
        reg__value[17] or
        reg__value[18] or
        reg__value[19] or
        reg__value[20] or
        reg__value[21] or
        reg__value[22] or
        reg__value[23] or
        reg__value[24] or
        reg__value[25] or
        reg__value[26] or
        reg__value[27] or
        reg__value[28] or
        reg__value[29] or
        reg__value[30] or
        reg__value[31] or
        reg__value[32] or
        reg__value[33] or
        reg__value[34] or
        reg__value[35] or
        reg__value[36] or
        reg__value[37] or
        reg__value[38] or
        reg__value[39] or
        reg__value[40] or
        reg__value[41] or
        reg__value[42] or
        reg__value[43] or
        reg__value[44] or
        reg__value[45] or
        reg__value[46] or
        reg__value[47] or
        reg__value[48] or
        reg__value[49] or
        reg__value[50] or
        reg__value[51] or
        reg__value[52] or
        reg__value[53] or
        reg__value[54] or
        reg__value[55] or
        reg__value[56] or
        reg__value[57] or
        reg__value[58] or
        reg__value[59] or
        reg__value[60] or
        reg__value[61] or
        reg__value[62] or
        reg__value[63]        //reg__value - Xilinx does not want arrays in sensitivity lists
 or
        inst )
    begin: the_code__comb_code
    reg [15:0]mem_add__var;
    reg [15:0]mem_in__var;
    reg mem_wr__var;
    reg mem_rd__var;
    reg mem_fetch__var;
    reg [3:0]astate_next__var;
    reg [15:0]pc_next__var;
    reg [16:0]op_a__var;
    reg [16:0]op_b__var;
    reg [16:0]op_res__var;
        mem_add__var = 16'h0;
        mem_in__var = 16'h0;
        mem_wr__var = 1'h0;
        mem_rd__var = 1'h0;
        mem_fetch__var = 1'h0;
        astate_next__var = astate;
        pc_next__var = reg__value[0];
        case (astate) //synopsys parallel_case
        4'h0: // req 1
            begin
            astate_next__var = 4'h1;
            end
        4'h1: // req 1
            begin
            pc_next__var = 16'h0;
            astate_next__var = 4'h2;
            end
        4'h2: // req 1
            begin
            pc_next__var = (reg__value[0]+16'h1);
            mem_add__var = reg__value[0];
            mem_fetch__var = 1'h1;
            mem_rd__var = 1'h1;
            astate_next__var = 4'h3;
            end
        4'h3: // req 1
            begin
            astate_next__var = 4'hf;
            case (inst[15:12]) //synopsys parallel_case
            4'h4: // req 1
                begin
                case (inst[3:0]) //synopsys parallel_case
                4'h8: // req 1
                    begin
                    mem_add__var = reg__value[{2'h0,inst[11:8]}];
                    mem_wr__var = 1'h1;
                    mem_in__var = reg__value[{2'h0,inst[7:4]}];
                    astate_next__var = 4'h4;
                    end
                default: // req 1
                    begin
                    astate_next__var = 4'hf;
                    end
                endcase
                end
            4'h6: // req 1
                begin
                case (inst[3:0]) //synopsys parallel_case
                4'h8: // req 1
                    begin
                    op_a__var = 17'h1;
                    op_b__var = {1'h0,reg__value[{2'h0,inst[7:4]}]};
                    op_res__var = (op_a__var+op_b__var);
                    astate_next__var = 4'h2;
                    end
                4'h9: // req 1
                    begin
                    op_a__var = {1'h0,reg__value[{2'h0,inst[7:4]}]};
                    op_b__var = 17'h1;
                    op_res__var = (op_a__var-op_b__var);
                    astate_next__var = 4'h2;
                    end
                default: // req 1
                    begin
                    astate_next__var = 4'hf;
                    end
                endcase
                end
            4'h8: // req 1
                begin
                astate_next__var = 4'h2;
                end
            4'h9: // req 1
                begin
                astate_next__var = 4'h2;
                end
            4'ha: // req 1
                begin
                astate_next__var = 4'h2;
                end
            4'hb: // req 1
                begin
                astate_next__var = 4'h2;
                end
            4'hc: // req 1
                begin
                case (inst[11:8]) //synopsys parallel_case
                4'h0: // req 1
                    begin
                    pc_next__var = (reg__value[0]+{{{{{{{{inst[7],inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7:0]});
                    astate_next__var = 4'h2;
                    end
                4'h1: // req 1
                    begin
                    if ((reg__value[1][0]==1'h1))
                    begin
                        pc_next__var = (reg__value[0]+{{{{{{{{inst[7],inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7:0]});
                    end //if
                    astate_next__var = 4'h2;
                    end
                4'h2: // req 1
                    begin
                    if ((reg__value[1][0]==1'h0))
                    begin
                        pc_next__var = (reg__value[0]+{{{{{{{{inst[7],inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7]},inst[7:0]});
                    end //if
                    astate_next__var = 4'h2;
                    end
                default: // req 1
                    begin
                    astate_next__var = 4'hf;
                    end
                endcase
                end
            default: // req 1
                begin
                astate_next__var = 4'hf;
                end
            endcase
            end
        4'h4: // req 1
            begin
            astate_next__var = 4'h2;
            end
        default: // req 1
            begin
            astate_next__var = 4'hf;
            end
        endcase
        mem_add = mem_add__var;
        mem_in = mem_in__var;
        mem_wr = mem_wr__var;
        mem_rd = mem_rd__var;
        mem_fetch = mem_fetch__var;
        astate_next = astate_next__var;
        pc_next = pc_next__var;
        op_a = op_a__var;
        op_b = op_b__var;
        op_res = op_res__var;
    end //always

    //b the_code__posedge_in_clock_active_low_in_reset clock process
    always @( posedge in_clock or negedge in_reset)
    begin : the_code__posedge_in_clock_active_low_in_reset__code
        if (in_reset==1'b0)
        begin
            inst <= 16'h0;
            reg__value[0] <= 16'h0;
            reg__value[1] <= 16'h0;
            reg__value[2] <= 16'h0;
            reg__value[3] <= 16'h0;
            reg__value[4] <= 16'h0;
            reg__value[5] <= 16'h0;
            reg__value[6] <= 16'h0;
            reg__value[7] <= 16'h0;
            reg__value[8] <= 16'h0;
            reg__value[9] <= 16'h0;
            reg__value[10] <= 16'h0;
            reg__value[11] <= 16'h0;
            reg__value[12] <= 16'h0;
            reg__value[13] <= 16'h0;
            reg__value[14] <= 16'h0;
            reg__value[15] <= 16'h0;
            reg__value[16] <= 16'h0;
            reg__value[17] <= 16'h0;
            reg__value[18] <= 16'h0;
            reg__value[19] <= 16'h0;
            reg__value[20] <= 16'h0;
            reg__value[21] <= 16'h0;
            reg__value[22] <= 16'h0;
            reg__value[23] <= 16'h0;
            reg__value[24] <= 16'h0;
            reg__value[25] <= 16'h0;
            reg__value[26] <= 16'h0;
            reg__value[27] <= 16'h0;
            reg__value[28] <= 16'h0;
            reg__value[29] <= 16'h0;
            reg__value[30] <= 16'h0;
            reg__value[31] <= 16'h0;
            reg__value[32] <= 16'h0;
            reg__value[33] <= 16'h0;
            reg__value[34] <= 16'h0;
            reg__value[35] <= 16'h0;
            reg__value[36] <= 16'h0;
            reg__value[37] <= 16'h0;
            reg__value[38] <= 16'h0;
            reg__value[39] <= 16'h0;
            reg__value[40] <= 16'h0;
            reg__value[41] <= 16'h0;
            reg__value[42] <= 16'h0;
            reg__value[43] <= 16'h0;
            reg__value[44] <= 16'h0;
            reg__value[45] <= 16'h0;
            reg__value[46] <= 16'h0;
            reg__value[47] <= 16'h0;
            reg__value[48] <= 16'h0;
            reg__value[49] <= 16'h0;
            reg__value[50] <= 16'h0;
            reg__value[51] <= 16'h0;
            reg__value[52] <= 16'h0;
            reg__value[53] <= 16'h0;
            reg__value[54] <= 16'h0;
            reg__value[55] <= 16'h0;
            reg__value[56] <= 16'h0;
            reg__value[57] <= 16'h0;
            reg__value[58] <= 16'h0;
            reg__value[59] <= 16'h0;
            reg__value[60] <= 16'h0;
            reg__value[61] <= 16'h0;
            reg__value[62] <= 16'h0;
            reg__value[63] <= 16'h0;
            astate <= 4'h0;
        end
        else
        begin
            case (astate) //synopsys parallel_case
            4'h0: // req 1
                begin
                end
            4'h1: // req 1
                begin
                end
            4'h2: // req 1
                begin
                inst <= mem_out;
                end
            4'h3: // req 1
                begin
                case (inst[15:12]) //synopsys parallel_case
                4'h4: // req 1
                    begin
                    end
                4'h6: // req 1
                    begin
                    case (inst[3:0]) //synopsys parallel_case
                    4'h8: // req 1
                        begin
                        reg__value[1][1] <= op_res[16];
                        reg__value[1][2] <= op_res[15];
                        if ((op_res[15:0]==16'h0))
                        begin
                            reg__value[1][0] <= 1'h1;
                        end //if
                        else
                        begin
                            reg__value[1][0] <= 1'h0;
                        end //else
                        if (((op_a[15]==op_b[15])&&(op_res[15]!=op_b[15])))
                        begin
                            reg__value[1][3] <= 1'h1;
                        end //if
                        else
                        begin
                            reg__value[1][3] <= 1'h0;
                        end //else
                        reg__value[{2'h0,inst[11:8]}] <= op_res[15:0];
                        end
                    4'h9: // req 1
                        begin
                        reg__value[1][1] <= ~op_res[16];
                        reg__value[1][2] <= op_res[15];
                        if ((op_res[15:0]==16'h0))
                        begin
                            reg__value[1][0] <= 1'h1;
                        end //if
                        else
                        begin
                            reg__value[1][0] <= 1'h0;
                        end //else
                        if (((op_a[15]!=op_b[15])&&(op_res[15]==op_b[15])))
                        begin
                            reg__value[1][3] <= 1'h1;
                        end //if
                        else
                        begin
                            reg__value[1][3] <= 1'h0;
                        end //else
                        reg__value[{2'h0,inst[11:8]}] <= op_res[15:0];
                        end
                    default: // req 1
                        begin
                        end
                    endcase
                    end
                4'h8: // req 1
                    begin
                    reg__value[{2'h0,inst[11:8]}][15:8] <= 8'h0;
                    reg__value[{2'h0,inst[11:8]}][7:0] <= inst[7:0];
                    end
                4'h9: // req 1
                    begin
                    reg__value[{2'h0,inst[11:8]}][15:8] <= inst[7:0];
                    reg__value[{2'h0,inst[11:8]}][7:0] <= 8'h0;
                    end
                4'ha: // req 1
                    begin
                    reg__value[{2'h0,inst[11:8]}][7:0] <= inst[7:0];
                    end
                4'hb: // req 1
                    begin
                    reg__value[{2'h0,inst[11:8]}][15:8] <= inst[7:0];
                    end
                4'hc: // req 1
                    begin
                    end
                default: // req 1
                    begin
                    end
                endcase
                end
            4'h4: // req 1
                begin
                end
            default: // req 1
                begin
                end
            endcase
            reg__value[0] <= pc_next;
            astate <= astate_next;
        end //if
    end //always

endmodule // lsa_core
