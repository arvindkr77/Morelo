#ifndef MO4_WF_ACTION_HANDLERS_IMPL_HXX
#define MO4_WF_ACTION_HANDLERS_IMPL_HXX

#include <epm/epm.h>



int MO4_ah_create_part_for_design
(
	EPM_action_message_t msg
);

int MO4_ah_create_design_for_part
(
	EPM_action_message_t msg
);

#endif

