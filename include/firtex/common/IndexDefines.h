#ifndef __FX_INDEXDEFINES_H
#define __FX_INDEXDEFINES_H

// define current index version
#define FX_INDEX_VERSION _T("firtex2-0.3.0")

//record size of doc/pos list
#define RECORD_SIZE 128
#define SKIP_INTERVAL 128
#define MAX_CHUNK_COUNT_PER_SEGMENT 10

// define file names
#define SCHEMA_FILENAME "schema"

#define BARREL_COMMIT_FILENAME_PREFIX "barrels.c"
#define BARREL_COMMIT_FILENAME_PREFIX_LENGTH 9
#define BARREL_DIRECTORY_PREFIX "b"

#define DELETION_FILENAME "deletions"
#define STORED_FIELDS_IDX_FILENAME "stored.idx"
#define STORED_FIELDS_VALUE_FILENAME "stored.val"

#define DICT_FILEEXT "dic"
#define DOC_POSTING_FILEEXT "dop"
#define POS_POSTING_FILEEXT "pop"
#define FORWARD_INDEX_FILEEXT "fdv"
#define NORM_FILEEXT "norm"
#define PRIMARYKEY_FILEEXT "pk"

#define BARREL_BUILDING_STATE_SUFFIX ".building."
#define BARREL_MERGING_STATE_SUFFIX ".merging."

#endif
