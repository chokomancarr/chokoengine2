#pragma once

/* Use this to define user scripts
 * that can be attached as components
 */
#define CE_SCRIPT_DEF(nm)\
	class _ ## nm : public CE_NS _Script

/* Append this to public classes
 */
#define CE_SERIALIZABLE

/* Append this to public variables
 * and public members of serializable classes
 */
#define CE_SERIALIZE
