/*
 * Copyright (c) 2012 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "features/storage/kvstore/include/KVStore.h"
#include "features/storage/kvstore/kv_map/KVMap.h"
#include "features/storage/kvstore/conf/kv_config.h"
#include "mbed_error.h"

namespace mbed {

extern "C" int kv_init_storage_config() {
    return MBED_SUCCESS;
}

kvstore_config_t testConfig;

KVMap::~KVMap()
{
    deinit();
}

int KVMap::init()
{
    return 0;
}

int KVMap::attach(const char *partition_name, kvstore_config_t *kv_config)
{
    return 0;
}

void KVMap::deinit_partition(kv_map_entry_t *partition)
{
}


int KVMap::detach(const char *partition_name)
{
    return MBED_SUCCESS;
}

int KVMap::deinit()
{
    return MBED_SUCCESS;
}

// Full name lookup and then break it into KVStore instance and key
int KVMap::lookup(const char *full_name, KVStore **kv_instance, size_t *key_index, uint32_t *flags_mask)
{
    return MBED_SUCCESS;
}

// Full name lookup and then break it into KVStore configuration struct and key
int KVMap::config_lookup(const char *full_name, kvstore_config_t **kv_config, size_t *key_index)
{
    return MBED_SUCCESS;
}

KVStore *KVMap::get_internal_kv_instance(const char *name)
{
    return testConfig.internal_store;
}

KVStore *KVMap::get_external_kv_instance(const char *name)
{
    return testConfig.external_store;
}

KVStore *KVMap::get_main_kv_instance(const char *name)
{
    return testConfig.kvstore_main_instance;
}

BlockDevice *KVMap::get_internal_blockdevice_instance(const char *name)
{
    return testConfig.internal_bd;
}

BlockDevice *KVMap::get_external_blockdevice_instance(const char *name)
{
    return testConfig.external_bd;
}

FileSystem *KVMap::get_external_filesystem_instance(const char *name)
{
    return testConfig.external_fs;
}

} // namespace mbed

