/* Copyright 2015 Baruch Even
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef LIBSCSICMD_LOG_SENSE_H
#define LIBSCSICMD_LOG_SENSE_H

#include "scsicmd_utils.h"
#include <stdint.h>
#include <stdbool.h>

/* Log Sense Header decode */

#define LOG_SENSE_MIN_LEN 4

static inline uint8_t log_sense_page_code(uint8_t *data)
{
	return data[0] & 0x3F;
}

static inline bool log_sense_subpage_format(uint8_t *data)
{
	return data[0] & 0x40;
}

static inline bool log_sense_data_saved(uint8_t *data)
{
	return data[0] & 0x80;
}

static inline uint8_t log_sense_subpage_code(uint8_t *data)
{
	return data[1];
}

static inline uint16_t log_sense_data_len(uint8_t *data)
{
	return get_uint16(data, 2);
}

static inline uint8_t *log_sense_data(uint8_t *data)
{
	return data + LOG_SENSE_MIN_LEN;
}

static inline bool log_sense_valid(unsigned data_len)
{
	if (data_len < LOG_SENSE_MIN_LEN)
		return false;
	return true;
}

/* Log Sense Parameter decode */
#define LOG_SENSE_MIN_PARAM_LEN 4

#define LOG_PARAM_FLAG_DU 0x80
#define LOG_PARAM_FLAG_TSD 0x20
#define LOG_PARAM_FLAG_ETC 0x10
#define LOG_PARAM_FLAG_TMC_MASK 0x0C
#define LOG_PARAM_FLAG_FMT_MASK 0x03

#define LOG_PARAM_TMC_EVERY_UPDATE 0
#define LOG_PARAM_TMC_EQUAL 1
#define LOG_PARAM_TMC_NOT_EQUAL 2
#define LOG_PARAM_TMC_GREATER 3

#define LOG_PARAM_FMT_COUNTER_STOP 0
#define LOG_PARAM_FMT_ASCII 1
#define LOG_PARAM_FMT_COUNTER_ROLLOVER 2
#define LOG_PARAM_FMT_BINARY 2

static inline uint16_t log_sense_param_code(uint8_t *param)
{
	return get_uint16(param, 0);
}

inline static uint8_t log_sense_param_flags(uint8_t *param)
{
	return param[2];
}

inline static uint8_t log_sense_param_tmc(uint8_t *param)
{
	return (log_sense_param_flags(param) & LOG_PARAM_FLAG_TMC_MASK) >> 2;
}

inline static uint8_t log_sense_param_fmt(uint8_t *param)
{
	return log_sense_param_flags(param) & LOG_PARAM_FLAG_FMT_MASK;
}

static inline uint8_t log_sense_param_len(uint8_t *param)
{
	return param[3];
}

static inline uint8_t *log_sense_param_data(uint8_t *param)
{
	return param + LOG_SENSE_MIN_PARAM_LEN;
}

#define for_all_log_sense_params(data, data_len, param) \
	for (param = log_sense_data(data); \
		 (param - data) < data_len && (param + LOG_SENSE_MIN_PARAM_LEN - data) < data_len && (param + LOG_SENSE_MIN_PARAM_LEN + log_sense_param_len(param) - data) < data_len; \
		 param = param + LOG_SENSE_MIN_PARAM_LEN + log_sense_param_len(param))

bool log_sense_page_informational_exceptions(uint8_t *page, unsigned page_len, uint8_t *asc, uint8_t *ascq, uint8_t *temperature);

#endif
