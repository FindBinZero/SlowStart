//
// Created by FindBin on 2024/6/9.
//

#pragma once

/// id consists of both base_id and user_id
/// user only needs to configure user_id
#define user_id (0x01)      /// Configuration id



/// do not alter
#define base_id (0x7a0)
#define sstart_final_id (base_id | user_id)     /// can interrupt is entered only when the value is equal to sstart_final_id
