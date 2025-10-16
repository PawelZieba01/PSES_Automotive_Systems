/* This header file does not have an include guard,
 * because it can be used multiple times to check the required OS versions.
*/

#include <Os_Version.h>


/* This OS application template is build for
 * EB tresos AutoCore OS 6.0.322.
 *
 * Parts of the application template, which is delivered together with the OS, depend on the OS implementation.
 * The following checks make sure that the OS application template is used together with
 * EB tresos AutoCore OS 6.0.322.
 *
 * To update the application template to a new OS version, make sure that at least all files
 * which include this file are updated to match the requirements of the new OS version.
 * This is mainly related to the preconditions on hardware setup which have to be met before calling StartOS().
 * These preconditions can be found by
 *  - comparing all your start-up code with the template code shipped with the new OS version
 *  - reading the manuals and architecture notes of the respective operating systems.
*/

#ifndef TMPL_REQ_OS_SW_MAJOR_VERSION
#define TMPL_REQ_OS_SW_MAJOR_VERSION	6U
#endif
#ifndef TMPL_REQ_OS_SW_MINOR_VERSION
#define TMPL_REQ_OS_SW_MINOR_VERSION	0U
#endif
#ifndef TMPL_REQ_OS_SW_PATCH_VERSION
#define TMPL_REQ_OS_SW_PATCH_VERSION	322U
#endif

#if (TMPL_REQ_OS_SW_MAJOR_VERSION != OS_SW_MAJOR_VERSION)
#error "This template code requires a different OS major version."
#endif
#if (TMPL_REQ_OS_SW_MINOR_VERSION != OS_SW_MINOR_VERSION)
#error "This template code requires a different OS minor version."
#endif
#if (TMPL_REQ_OS_SW_PATCH_VERSION != OS_SW_PATCH_VERSION)
#error "This template code requires a different OS patch version."
#endif
