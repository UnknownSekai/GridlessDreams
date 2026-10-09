#pragma once
#include <map>
#include <string>
#include <unordered_map>

// models/unions.py: the IDataObject / INotificationObject union discriminator
// tables. wire::union_entry() takes the key already resolved, so these maps are
// the only place that names a discriminator key for an entity type.
namespace unions {

// discriminator key -> entity name. std::map iterates in ascending-key order,
// which is the Python insertion order (dict built 0..N), so iterating IDATA_OBJECT
// reproduces IDATA_OBJECT_KEY.items() order (used to build user_data's registry).
extern const std::map<int, std::string> IDATA_OBJECT;
extern const std::map<int, std::string> INOTIFICATION_OBJECT;

// entity name -> discriminator key, for IDATA_OBJECT_KEY[name] style lookups
extern const std::unordered_map<std::string, int> IDATA_OBJECT_KEY;
extern const std::unordered_map<std::string, int> INOTIFICATION_OBJECT_KEY;

}  // namespace unions
