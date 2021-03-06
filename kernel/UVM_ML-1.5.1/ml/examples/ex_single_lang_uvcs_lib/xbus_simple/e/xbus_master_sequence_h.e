/*------------------------------------------------------------------------- 
File name   : xbus_master_sequence_h.e
Title       : Sequence interface for ACTIVE master agents
Project     : XBus UVC
Created     : 2008
Description : This file provides a sequence interface for the master.
Notes       : 
--------------------------------------------------------------------------- 
-------------------------------------------------------------------------*/ 

<'

package cdn_xbus;

-- The following code adds fields to transfer struct for use when it is
-- generated by a master sequence.
extend MASTER xbus_trans_s {

    -- This field holds the logical name of the physical bus. This field is
    -- automatically constrained by the UVC and should not be constrained by
    -- the user.
    bus_name : xbus_bus_name_t;
        keep bus_name == read_only(driver.bus_name);    
   
    -- This field is used to sub-type the transfer struct according to which
    -- master agent it is for. This field is automatically constrained by the
    -- UVC and should not be constrained by the user.
    keep master_name == read_only(driver).master_name;

    -- Next fields define how the BFM should send the transfer
    wait_states : list of uint (bits : 4);
       keep wait_states.size() == size;
       keep for each in wait_states  {
           soft it == 9;
       };
    
    -- If this field is TRUE, then the error behaviour of this transfer is
    -- checked against the error_pos field.
    check_error : bool;
        keep soft check_error == TRUE;

    -- When the check_error field is TRUE, this field controls the expected
    -- error position. UNDEF means that no error is expected. There is a
    -- hard constraint that ensures that this field does not indicate an
    -- error position that is beyond the size of the transfer.
    error_pos_master : int;
        keep soft error_pos_master == UNDEF;
        keep error_pos_master < read_only(size);
    
    -- This field controls the delay from the end of the previous transfer to
    -- requesting the bus for this transfer in clock cycles.
    transmit_delay : uint;
        keep soft transmit_delay in [0..10];
        
}; -- extend MASTER xbus_trans_s {
   
        

-- This struct is the generic sequence for the master agent sequence
-- interface.
sequence xbus_master_sequence using 
        item = MASTER xbus_trans_s,
        created_driver = xbus_master_driver_u;

extend xbus_master_driver_u {

    !synch : xbus_synchronizer_u;
   
    event clock is only @synch.unqualified_clock_rise;
    
    -- This field holds the logical name of the physical bus. This field is
    -- automatically constrained by the UVC and should not be constrained by
    -- the user.
    bus_name : xbus_bus_name_t;
   
    -- This field is the logical name of the master. This
    -- field is automatically constrained by the UVC and should not be
    -- constrained by the user.
    master_name : xbus_agent_name_t;

}; -- extend xbus_master_driver_u


extend xbus_master_sequence {

    -- This field holds the logical name of the physical bus. This field is
    -- automatically constrained and should not be constrained by
    -- the user.
    bus_name : xbus_bus_name_t;
        keep bus_name == read_only(driver.bus_name);    
   
    -- This field holds the logical name of the master. This field is
    -- automatically constrained and should not be constrained by
    -- the user.
    master_name : xbus_agent_name_t;
        keep master_name == read_only(driver.master_name);
    
    -- This is a utility field for basic sequences. This allows the user to
    -- do "do trans ...".
    !trans       : MASTER xbus_trans_s;

    // Cover the sequence. 
    // Ignore the pre-defined kinds, they do not add info to the coverage
    cover ended using per_unit_instance=xbus_master_driver_u is {
        item kind using ignore = (kind == RANDOM or kind == MAIN);
    }; 

    -- This method performs one write transfer on the bus. It can be used in
    -- sequences to make them more readable.
    write(size : uint, 
          addr : xbus_addr_t, 
          data : uint(bits:64) ) @driver.clock is {
        if (size not in [1, 2, 4, 8]) {
            error("ERROR: xbus_master_driver:write() got size parameter ",
                  "not in [1, 2, 4, 8]");
        };
        
        trans = new with {
            .master_name = master_name;
            .slave_name  = NO_AGENT;
            .addr        = addr;
            .read_write  = WRITE;
            .size        = size;
            .data        = pack(packing.low, data[size*8-1:0]); 
        };
        driver.execute_item(trans);
    }; -- write()

    -- This method performs one read transfer on the bus. It can be used in
    -- sequences to make them more readable.
    read(size : uint, 
         addr : xbus_addr_t )      : uint(bits:64) @driver.clock is {
        if (size not in [1, 2, 4, 8]) {
            error("ERROR: xbus_master_driver:read() got size parameter ",
                  "not in [1, 2, 4, 8]");
        };
        trans = new with {
            .master_name = master_name;
            .slave_name  = NO_AGENT;
            .addr        = addr;
            .read_write  = READ;
            .size        = size;
        };
        trans.data.resize(size);
        driver.execute_item(trans);
        
        if trans != NULL {
            result = pack(packing.low, trans.data);
        } else {
            out("trans is NULL");
        };
    }; -- read()
}; -- extend xbus_master_sequence

'>

<'
extend MAIN xbus_master_sequence {
    keep soft count == 0;
};
'>
