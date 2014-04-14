/*
 * Define command snippets for saving and restoring env vars across reset
 */

#ifndef __CONFIG_NI_RESET_ENV_H
#define __CONFIG_NI_RESET_ENV_H

/* In order to make the resetenv command chain through the correct status, we
 * have to avoid saving and restoring environment variables that don't exist
 * across all targets. To do so, we define save and restore macros for each
 * target type and use these macros to form the resetenv command. */

#define ENV_SAVE(var) #var "_save=$" #var " && "
#define ENV_RESTORE(var) "env set .flags " #var " && env set " #var " $" #var "_save && "

#endif /* __CONFIG_NI_RESET_ENV_H */
