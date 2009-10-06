/*
 * Copyright 2009 HIROSE, Masaaki
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

/*
 * mod_bumpy_life.c -- set MaxRequestsPerChild randomly
 *                     between MaxRequestsPerChildMin and MaxRequestsPerChildMax
 *
 * please refer to our project site for overview, installation,
 * configuration and more.
 *
 * http://github.com/hirose31/ap-mod_bumpy_life
 *
 */

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

#include "http_log.h"
#include "mpm_common.h"

#include <sys/types.h>
#include <unistd.h>

#ifdef DEBUG
  #define _dprint(s)         ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, s)
  #define _dprintf(f,s)      ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, r, f, s)
  #define _dprintp(f,s)      ap_log_perror(APLOG_MARK, APLOG_ERR, 0, p, f, s)
  #define _dprintpx(p,f,...) ap_log_perror(APLOG_MARK, APLOG_ERR, 0, p, f, __VA_ARGS__)
#else
  #define _dprint(s)
  #define _dprintf(f,s)
  #define _dprintp(f,s)
  #define _dprintpx(f,s,...)
#endif

static const char identity[] = "mod_bumpy_life version 0.1 $";

module AP_MODULE_DECLARE_DATA bumpy_life_module;

typedef struct {
    unsigned int min;
    unsigned int max;
} bumpy_life_conf;

static void *create_config(apr_pool_t *p)
{
    bumpy_life_conf *conf = (bumpy_life_conf *)apr_palloc(p, sizeof(bumpy_life_conf));
    conf->min = ap_max_requests_per_child;
    conf->max = ap_max_requests_per_child + 1;
    return (void *)conf;
}

static void *create_bumpy_life_svr_config(apr_pool_t* p, server_rec* s)
{
  return create_config(p);
}

static const char *set_mrpc_min(cmd_parms *cmd, void *_conf,
                                const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if (err != NULL) {
        return err;
    }

    bumpy_life_conf *conf = ap_get_module_config(cmd->server->module_config,
                                                 &bumpy_life_module);
    conf->min = atoi(arg);
    return NULL;
}

static const char *set_mrpc_max(cmd_parms *cmd, void *_conf,
                                const char *arg)
{
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if (err != NULL) {
        return err;
    }

    bumpy_life_conf *conf = ap_get_module_config(cmd->server->module_config,
                                                 &bumpy_life_module);
    conf->max = atoi(arg);
    return NULL;
}

static const command_rec bumpy_life_cmds[] =
{
    AP_INIT_TAKE1("MaxRequestsPerChildMin", set_mrpc_min,
                  NULL,
                  RSRC_CONF,
                  "fixme"),
    AP_INIT_TAKE1("MaxRequestsPerChildMax", set_mrpc_max,
                  NULL,
                  RSRC_CONF,
                  "fixme"),
    { NULL },
};

static void child_init_handler(apr_pool_t *p, server_rec *s)
{
    bumpy_life_conf* conf =
        (bumpy_life_conf*)ap_get_module_config(s->module_config, &bumpy_life_module);

    srand(time(NULL) ^ getpid());
    ap_max_requests_per_child = conf->min + rand() % (conf->max - conf->min);

    _dprintp("bumpy_life_conf->min: %d", conf->min);
    _dprintp("bumpy_life_conf->max: %d", conf->max);
    _dprintp("ap_max_requests_per_child=%d", ap_max_requests_per_child);
}

static void bumpy_life_register_hooks(apr_pool_t *p)
{
    ap_hook_child_init(child_init_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA bumpy_life_module = {
    STANDARD20_MODULE_STUFF,
    NULL,                         /* create per-dir    config structures */
    NULL,                         /* merge  per-dir    config structures */
    create_bumpy_life_svr_config, /* create per-server config structures */
    NULL,                         /* merge  per-server config structures */
    bumpy_life_cmds,              /* table of config file commands       */
    bumpy_life_register_hooks     /* register hooks                      */
};
