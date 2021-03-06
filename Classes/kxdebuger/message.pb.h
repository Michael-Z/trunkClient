// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message.proto

#ifndef PROTOBUF_message_2eproto__INCLUDED
#define PROTOBUF_message_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace kxdebuger {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_message_2eproto();
void protobuf_AssignDesc_message_2eproto();
void protobuf_ShutdownFile_message_2eproto();

class NodeTree;
class Info;
class InfoGroup;
class NodeInfo;
class NodeId;

// ===================================================================

class NodeTree : public ::google::protobuf::MessageLite {
 public:
  NodeTree();
  virtual ~NodeTree();
  
  NodeTree(const NodeTree& from);
  
  inline NodeTree& operator=(const NodeTree& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const NodeTree& default_instance();
  
  void Swap(NodeTree* other);
  
  // implements Message ----------------------------------------------
  
  NodeTree* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const NodeTree& from);
  void MergeFrom(const NodeTree& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 nodeId = 1;
  inline bool has_nodeid() const;
  inline void clear_nodeid();
  static const int kNodeIdFieldNumber = 1;
  inline ::google::protobuf::int32 nodeid() const;
  inline void set_nodeid(::google::protobuf::int32 value);
  
  // optional string name = 2;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 2;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  
  // repeated .kxdebuger.NodeTree children = 3;
  inline int children_size() const;
  inline void clear_children();
  static const int kChildrenFieldNumber = 3;
  inline const ::kxdebuger::NodeTree& children(int index) const;
  inline ::kxdebuger::NodeTree* mutable_children(int index);
  inline ::kxdebuger::NodeTree* add_children();
  inline const ::google::protobuf::RepeatedPtrField< ::kxdebuger::NodeTree >&
      children() const;
  inline ::google::protobuf::RepeatedPtrField< ::kxdebuger::NodeTree >*
      mutable_children();
  
  // @@protoc_insertion_point(class_scope:kxdebuger.NodeTree)
 private:
  inline void set_has_nodeid();
  inline void clear_has_nodeid();
  inline void set_has_name();
  inline void clear_has_name();
  
  ::std::string* name_;
  ::google::protobuf::RepeatedPtrField< ::kxdebuger::NodeTree > children_;
  ::google::protobuf::int32 nodeid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
  void InitAsDefaultInstance();
  static NodeTree* default_instance_;
};
// -------------------------------------------------------------------

class Info : public ::google::protobuf::MessageLite {
 public:
  Info();
  virtual ~Info();
  
  Info(const Info& from);
  
  inline Info& operator=(const Info& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const Info& default_instance();
  
  void Swap(Info* other);
  
  // implements Message ----------------------------------------------
  
  Info* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const Info& from);
  void MergeFrom(const Info& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 attributeType = 1;
  inline bool has_attributetype() const;
  inline void clear_attributetype();
  static const int kAttributeTypeFieldNumber = 1;
  inline ::google::protobuf::int32 attributetype() const;
  inline void set_attributetype(::google::protobuf::int32 value);
  
  // required int32 valueType = 2;
  inline bool has_valuetype() const;
  inline void clear_valuetype();
  static const int kValueTypeFieldNumber = 2;
  inline ::google::protobuf::int32 valuetype() const;
  inline void set_valuetype(::google::protobuf::int32 value);
  
  // required string value = 3;
  inline bool has_value() const;
  inline void clear_value();
  static const int kValueFieldNumber = 3;
  inline const ::std::string& value() const;
  inline void set_value(const ::std::string& value);
  inline void set_value(const char* value);
  inline void set_value(const char* value, size_t size);
  inline ::std::string* mutable_value();
  inline ::std::string* release_value();
  
  // @@protoc_insertion_point(class_scope:kxdebuger.Info)
 private:
  inline void set_has_attributetype();
  inline void clear_has_attributetype();
  inline void set_has_valuetype();
  inline void clear_has_valuetype();
  inline void set_has_value();
  inline void clear_has_value();
  
  ::google::protobuf::int32 attributetype_;
  ::google::protobuf::int32 valuetype_;
  ::std::string* value_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
  void InitAsDefaultInstance();
  static Info* default_instance_;
};
// -------------------------------------------------------------------

class InfoGroup : public ::google::protobuf::MessageLite {
 public:
  InfoGroup();
  virtual ~InfoGroup();
  
  InfoGroup(const InfoGroup& from);
  
  inline InfoGroup& operator=(const InfoGroup& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const InfoGroup& default_instance();
  
  void Swap(InfoGroup* other);
  
  // implements Message ----------------------------------------------
  
  InfoGroup* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const InfoGroup& from);
  void MergeFrom(const InfoGroup& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string groupName = 1;
  inline bool has_groupname() const;
  inline void clear_groupname();
  static const int kGroupNameFieldNumber = 1;
  inline const ::std::string& groupname() const;
  inline void set_groupname(const ::std::string& value);
  inline void set_groupname(const char* value);
  inline void set_groupname(const char* value, size_t size);
  inline ::std::string* mutable_groupname();
  inline ::std::string* release_groupname();
  
  // repeated .kxdebuger.Info attributes = 2;
  inline int attributes_size() const;
  inline void clear_attributes();
  static const int kAttributesFieldNumber = 2;
  inline const ::kxdebuger::Info& attributes(int index) const;
  inline ::kxdebuger::Info* mutable_attributes(int index);
  inline ::kxdebuger::Info* add_attributes();
  inline const ::google::protobuf::RepeatedPtrField< ::kxdebuger::Info >&
      attributes() const;
  inline ::google::protobuf::RepeatedPtrField< ::kxdebuger::Info >*
      mutable_attributes();
  
  // @@protoc_insertion_point(class_scope:kxdebuger.InfoGroup)
 private:
  inline void set_has_groupname();
  inline void clear_has_groupname();
  
  ::std::string* groupname_;
  ::google::protobuf::RepeatedPtrField< ::kxdebuger::Info > attributes_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
  void InitAsDefaultInstance();
  static InfoGroup* default_instance_;
};
// -------------------------------------------------------------------

class NodeInfo : public ::google::protobuf::MessageLite {
 public:
  NodeInfo();
  virtual ~NodeInfo();
  
  NodeInfo(const NodeInfo& from);
  
  inline NodeInfo& operator=(const NodeInfo& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const NodeInfo& default_instance();
  
  void Swap(NodeInfo* other);
  
  // implements Message ----------------------------------------------
  
  NodeInfo* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const NodeInfo& from);
  void MergeFrom(const NodeInfo& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 nodeId = 1;
  inline bool has_nodeid() const;
  inline void clear_nodeid();
  static const int kNodeIdFieldNumber = 1;
  inline ::google::protobuf::int32 nodeid() const;
  inline void set_nodeid(::google::protobuf::int32 value);
  
  // repeated .kxdebuger.InfoGroup attributes = 2;
  inline int attributes_size() const;
  inline void clear_attributes();
  static const int kAttributesFieldNumber = 2;
  inline const ::kxdebuger::InfoGroup& attributes(int index) const;
  inline ::kxdebuger::InfoGroup* mutable_attributes(int index);
  inline ::kxdebuger::InfoGroup* add_attributes();
  inline const ::google::protobuf::RepeatedPtrField< ::kxdebuger::InfoGroup >&
      attributes() const;
  inline ::google::protobuf::RepeatedPtrField< ::kxdebuger::InfoGroup >*
      mutable_attributes();
  
  // @@protoc_insertion_point(class_scope:kxdebuger.NodeInfo)
 private:
  inline void set_has_nodeid();
  inline void clear_has_nodeid();
  
  ::google::protobuf::RepeatedPtrField< ::kxdebuger::InfoGroup > attributes_;
  ::google::protobuf::int32 nodeid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
  void InitAsDefaultInstance();
  static NodeInfo* default_instance_;
};
// -------------------------------------------------------------------

class NodeId : public ::google::protobuf::MessageLite {
 public:
  NodeId();
  virtual ~NodeId();
  
  NodeId(const NodeId& from);
  
  inline NodeId& operator=(const NodeId& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const NodeId& default_instance();
  
  void Swap(NodeId* other);
  
  // implements Message ----------------------------------------------
  
  NodeId* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const NodeId& from);
  void MergeFrom(const NodeId& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required int32 nodeId = 1;
  inline bool has_nodeid() const;
  inline void clear_nodeid();
  static const int kNodeIdFieldNumber = 1;
  inline ::google::protobuf::int32 nodeid() const;
  inline void set_nodeid(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:kxdebuger.NodeId)
 private:
  inline void set_has_nodeid();
  inline void clear_has_nodeid();
  
  ::google::protobuf::int32 nodeid_;
  
  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
  friend void  protobuf_AddDesc_message_2eproto();
  friend void protobuf_AssignDesc_message_2eproto();
  friend void protobuf_ShutdownFile_message_2eproto();
  
  void InitAsDefaultInstance();
  static NodeId* default_instance_;
};
// ===================================================================


// ===================================================================

// NodeTree

// required int32 nodeId = 1;
inline bool NodeTree::has_nodeid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void NodeTree::set_has_nodeid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void NodeTree::clear_has_nodeid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void NodeTree::clear_nodeid() {
  nodeid_ = 0;
  clear_has_nodeid();
}
inline ::google::protobuf::int32 NodeTree::nodeid() const {
  return nodeid_;
}
inline void NodeTree::set_nodeid(::google::protobuf::int32 value) {
  set_has_nodeid();
  nodeid_ = value;
}

// optional string name = 2;
inline bool NodeTree::has_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void NodeTree::set_has_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void NodeTree::clear_has_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void NodeTree::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& NodeTree::name() const {
  return *name_;
}
inline void NodeTree::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void NodeTree::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void NodeTree::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* NodeTree::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* NodeTree::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated .kxdebuger.NodeTree children = 3;
inline int NodeTree::children_size() const {
  return children_.size();
}
inline void NodeTree::clear_children() {
  children_.Clear();
}
inline const ::kxdebuger::NodeTree& NodeTree::children(int index) const {
  return children_.Get(index);
}
inline ::kxdebuger::NodeTree* NodeTree::mutable_children(int index) {
  return children_.Mutable(index);
}
inline ::kxdebuger::NodeTree* NodeTree::add_children() {
  return children_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::kxdebuger::NodeTree >&
NodeTree::children() const {
  return children_;
}
inline ::google::protobuf::RepeatedPtrField< ::kxdebuger::NodeTree >*
NodeTree::mutable_children() {
  return &children_;
}

// -------------------------------------------------------------------

// Info

// required int32 attributeType = 1;
inline bool Info::has_attributetype() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Info::set_has_attributetype() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Info::clear_has_attributetype() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Info::clear_attributetype() {
  attributetype_ = 0;
  clear_has_attributetype();
}
inline ::google::protobuf::int32 Info::attributetype() const {
  return attributetype_;
}
inline void Info::set_attributetype(::google::protobuf::int32 value) {
  set_has_attributetype();
  attributetype_ = value;
}

// required int32 valueType = 2;
inline bool Info::has_valuetype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Info::set_has_valuetype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Info::clear_has_valuetype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Info::clear_valuetype() {
  valuetype_ = 0;
  clear_has_valuetype();
}
inline ::google::protobuf::int32 Info::valuetype() const {
  return valuetype_;
}
inline void Info::set_valuetype(::google::protobuf::int32 value) {
  set_has_valuetype();
  valuetype_ = value;
}

// required string value = 3;
inline bool Info::has_value() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Info::set_has_value() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Info::clear_has_value() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Info::clear_value() {
  if (value_ != &::google::protobuf::internal::kEmptyString) {
    value_->clear();
  }
  clear_has_value();
}
inline const ::std::string& Info::value() const {
  return *value_;
}
inline void Info::set_value(const ::std::string& value) {
  set_has_value();
  if (value_ == &::google::protobuf::internal::kEmptyString) {
    value_ = new ::std::string;
  }
  value_->assign(value);
}
inline void Info::set_value(const char* value) {
  set_has_value();
  if (value_ == &::google::protobuf::internal::kEmptyString) {
    value_ = new ::std::string;
  }
  value_->assign(value);
}
inline void Info::set_value(const char* value, size_t size) {
  set_has_value();
  if (value_ == &::google::protobuf::internal::kEmptyString) {
    value_ = new ::std::string;
  }
  value_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Info::mutable_value() {
  set_has_value();
  if (value_ == &::google::protobuf::internal::kEmptyString) {
    value_ = new ::std::string;
  }
  return value_;
}
inline ::std::string* Info::release_value() {
  clear_has_value();
  if (value_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = value_;
    value_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// -------------------------------------------------------------------

// InfoGroup

// required string groupName = 1;
inline bool InfoGroup::has_groupname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void InfoGroup::set_has_groupname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void InfoGroup::clear_has_groupname() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void InfoGroup::clear_groupname() {
  if (groupname_ != &::google::protobuf::internal::kEmptyString) {
    groupname_->clear();
  }
  clear_has_groupname();
}
inline const ::std::string& InfoGroup::groupname() const {
  return *groupname_;
}
inline void InfoGroup::set_groupname(const ::std::string& value) {
  set_has_groupname();
  if (groupname_ == &::google::protobuf::internal::kEmptyString) {
    groupname_ = new ::std::string;
  }
  groupname_->assign(value);
}
inline void InfoGroup::set_groupname(const char* value) {
  set_has_groupname();
  if (groupname_ == &::google::protobuf::internal::kEmptyString) {
    groupname_ = new ::std::string;
  }
  groupname_->assign(value);
}
inline void InfoGroup::set_groupname(const char* value, size_t size) {
  set_has_groupname();
  if (groupname_ == &::google::protobuf::internal::kEmptyString) {
    groupname_ = new ::std::string;
  }
  groupname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* InfoGroup::mutable_groupname() {
  set_has_groupname();
  if (groupname_ == &::google::protobuf::internal::kEmptyString) {
    groupname_ = new ::std::string;
  }
  return groupname_;
}
inline ::std::string* InfoGroup::release_groupname() {
  clear_has_groupname();
  if (groupname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = groupname_;
    groupname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}

// repeated .kxdebuger.Info attributes = 2;
inline int InfoGroup::attributes_size() const {
  return attributes_.size();
}
inline void InfoGroup::clear_attributes() {
  attributes_.Clear();
}
inline const ::kxdebuger::Info& InfoGroup::attributes(int index) const {
  return attributes_.Get(index);
}
inline ::kxdebuger::Info* InfoGroup::mutable_attributes(int index) {
  return attributes_.Mutable(index);
}
inline ::kxdebuger::Info* InfoGroup::add_attributes() {
  return attributes_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::kxdebuger::Info >&
InfoGroup::attributes() const {
  return attributes_;
}
inline ::google::protobuf::RepeatedPtrField< ::kxdebuger::Info >*
InfoGroup::mutable_attributes() {
  return &attributes_;
}

// -------------------------------------------------------------------

// NodeInfo

// required int32 nodeId = 1;
inline bool NodeInfo::has_nodeid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void NodeInfo::set_has_nodeid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void NodeInfo::clear_has_nodeid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void NodeInfo::clear_nodeid() {
  nodeid_ = 0;
  clear_has_nodeid();
}
inline ::google::protobuf::int32 NodeInfo::nodeid() const {
  return nodeid_;
}
inline void NodeInfo::set_nodeid(::google::protobuf::int32 value) {
  set_has_nodeid();
  nodeid_ = value;
}

// repeated .kxdebuger.InfoGroup attributes = 2;
inline int NodeInfo::attributes_size() const {
  return attributes_.size();
}
inline void NodeInfo::clear_attributes() {
  attributes_.Clear();
}
inline const ::kxdebuger::InfoGroup& NodeInfo::attributes(int index) const {
  return attributes_.Get(index);
}
inline ::kxdebuger::InfoGroup* NodeInfo::mutable_attributes(int index) {
  return attributes_.Mutable(index);
}
inline ::kxdebuger::InfoGroup* NodeInfo::add_attributes() {
  return attributes_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::kxdebuger::InfoGroup >&
NodeInfo::attributes() const {
  return attributes_;
}
inline ::google::protobuf::RepeatedPtrField< ::kxdebuger::InfoGroup >*
NodeInfo::mutable_attributes() {
  return &attributes_;
}

// -------------------------------------------------------------------

// NodeId

// required int32 nodeId = 1;
inline bool NodeId::has_nodeid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void NodeId::set_has_nodeid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void NodeId::clear_has_nodeid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void NodeId::clear_nodeid() {
  nodeid_ = 0;
  clear_has_nodeid();
}
inline ::google::protobuf::int32 NodeId::nodeid() const {
  return nodeid_;
}
inline void NodeId::set_nodeid(::google::protobuf::int32 value) {
  set_has_nodeid();
  nodeid_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace kxdebuger

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_message_2eproto__INCLUDED
