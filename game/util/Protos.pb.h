// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Protos.proto

#ifndef PROTOBUF_Protos_2eproto__INCLUDED
#define PROTOBUF_Protos_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace protos {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Protos_2eproto();
void protobuf_AssignDesc_Protos_2eproto();
void protobuf_ShutdownFile_Protos_2eproto();

class Event;
class Message;
class Message_GameObject;

enum Event_Type {
  Event_Type_SPAWN = 1,
  Event_Type_MOVE = 2,
  Event_Type_JUMP = 3,
  Event_Type_ASSIGN = 4
};
bool Event_Type_IsValid(int value);
const Event_Type Event_Type_Type_MIN = Event_Type_SPAWN;
const Event_Type Event_Type_Type_MAX = Event_Type_ASSIGN;
const int Event_Type_Type_ARRAYSIZE = Event_Type_Type_MAX + 1;

const ::google::protobuf::EnumDescriptor* Event_Type_descriptor();
inline const ::std::string& Event_Type_Name(Event_Type value) {
  return ::google::protobuf::internal::NameOfEnum(
    Event_Type_descriptor(), value);
}
inline bool Event_Type_Parse(
    const ::std::string& name, Event_Type* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Event_Type>(
    Event_Type_descriptor(), name, value);
}
enum Event_Direction {
  Event_Direction_UP = 1,
  Event_Direction_DOWN = 2,
  Event_Direction_LEFT = 3,
  Event_Direction_RIGHT = 4,
  Event_Direction_FORWARD = 5,
  Event_Direction_BACKWARD = 6
};
bool Event_Direction_IsValid(int value);
const Event_Direction Event_Direction_Direction_MIN = Event_Direction_UP;
const Event_Direction Event_Direction_Direction_MAX = Event_Direction_BACKWARD;
const int Event_Direction_Direction_ARRAYSIZE = Event_Direction_Direction_MAX + 1;

const ::google::protobuf::EnumDescriptor* Event_Direction_descriptor();
inline const ::std::string& Event_Direction_Name(Event_Direction value) {
  return ::google::protobuf::internal::NameOfEnum(
    Event_Direction_descriptor(), value);
}
inline bool Event_Direction_Parse(
    const ::std::string& name, Event_Direction* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Event_Direction>(
    Event_Direction_descriptor(), name, value);
}
// ===================================================================

class Event : public ::google::protobuf::Message {
 public:
  Event();
  virtual ~Event();

  Event(const Event& from);

  inline Event& operator=(const Event& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Event& default_instance();

  void Swap(Event* other);

  // implements Message ----------------------------------------------

  Event* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Event& from);
  void MergeFrom(const Event& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef Event_Type Type;
  static const Type SPAWN = Event_Type_SPAWN;
  static const Type MOVE = Event_Type_MOVE;
  static const Type JUMP = Event_Type_JUMP;
  static const Type ASSIGN = Event_Type_ASSIGN;
  static inline bool Type_IsValid(int value) {
    return Event_Type_IsValid(value);
  }
  static const Type Type_MIN =
    Event_Type_Type_MIN;
  static const Type Type_MAX =
    Event_Type_Type_MAX;
  static const int Type_ARRAYSIZE =
    Event_Type_Type_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Type_descriptor() {
    return Event_Type_descriptor();
  }
  static inline const ::std::string& Type_Name(Type value) {
    return Event_Type_Name(value);
  }
  static inline bool Type_Parse(const ::std::string& name,
      Type* value) {
    return Event_Type_Parse(name, value);
  }

  typedef Event_Direction Direction;
  static const Direction UP = Event_Direction_UP;
  static const Direction DOWN = Event_Direction_DOWN;
  static const Direction LEFT = Event_Direction_LEFT;
  static const Direction RIGHT = Event_Direction_RIGHT;
  static const Direction FORWARD = Event_Direction_FORWARD;
  static const Direction BACKWARD = Event_Direction_BACKWARD;
  static inline bool Direction_IsValid(int value) {
    return Event_Direction_IsValid(value);
  }
  static const Direction Direction_MIN =
    Event_Direction_Direction_MIN;
  static const Direction Direction_MAX =
    Event_Direction_Direction_MAX;
  static const int Direction_ARRAYSIZE =
    Event_Direction_Direction_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Direction_descriptor() {
    return Event_Direction_descriptor();
  }
  static inline const ::std::string& Direction_Name(Direction value) {
    return Event_Direction_Name(value);
  }
  static inline bool Direction_Parse(const ::std::string& name,
      Direction* value) {
    return Event_Direction_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional .protos.Event.Type type = 2;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 2;
  inline ::protos::Event_Type type() const;
  inline void set_type(::protos::Event_Type value);

  // optional int32 clientID = 6;
  inline bool has_clientid() const;
  inline void clear_clientid();
  static const int kClientIDFieldNumber = 6;
  inline ::google::protobuf::int32 clientid() const;
  inline void set_clientid(::google::protobuf::int32 value);

  // optional .protos.Event.Direction direction = 7;
  inline bool has_direction() const;
  inline void clear_direction();
  static const int kDirectionFieldNumber = 7;
  inline ::protos::Event_Direction direction() const;
  inline void set_direction(::protos::Event_Direction value);

  // @@protoc_insertion_point(class_scope:protos.Event)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_type();
  inline void clear_has_type();
  inline void set_has_clientid();
  inline void clear_has_clientid();
  inline void set_has_direction();
  inline void clear_has_direction();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 id_;
  int type_;
  ::google::protobuf::int32 clientid_;
  int direction_;
  friend void  protobuf_AddDesc_Protos_2eproto();
  friend void protobuf_AssignDesc_Protos_2eproto();
  friend void protobuf_ShutdownFile_Protos_2eproto();

  void InitAsDefaultInstance();
  static Event* default_instance_;
};
// -------------------------------------------------------------------

class Message_GameObject : public ::google::protobuf::Message {
 public:
  Message_GameObject();
  virtual ~Message_GameObject();

  Message_GameObject(const Message_GameObject& from);

  inline Message_GameObject& operator=(const Message_GameObject& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Message_GameObject& default_instance();

  void Swap(Message_GameObject* other);

  // implements Message ----------------------------------------------

  Message_GameObject* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Message_GameObject& from);
  void MergeFrom(const Message_GameObject& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // repeated double matrix = 2;
  inline int matrix_size() const;
  inline void clear_matrix();
  static const int kMatrixFieldNumber = 2;
  inline double matrix(int index) const;
  inline void set_matrix(int index, double value);
  inline void add_matrix(double value);
  inline const ::google::protobuf::RepeatedField< double >&
      matrix() const;
  inline ::google::protobuf::RepeatedField< double >*
      mutable_matrix();

  // @@protoc_insertion_point(class_scope:protos.Message.GameObject)
 private:
  inline void set_has_id();
  inline void clear_has_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedField< double > matrix_;
  ::google::protobuf::int32 id_;
  friend void  protobuf_AddDesc_Protos_2eproto();
  friend void protobuf_AssignDesc_Protos_2eproto();
  friend void protobuf_ShutdownFile_Protos_2eproto();

  void InitAsDefaultInstance();
  static Message_GameObject* default_instance_;
};
// -------------------------------------------------------------------

class Message : public ::google::protobuf::Message {
 public:
  Message();
  virtual ~Message();

  Message(const Message& from);

  inline Message& operator=(const Message& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Message& default_instance();

  void Swap(Message* other);

  // implements Message ----------------------------------------------

  Message* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Message& from);
  void MergeFrom(const Message& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef Message_GameObject GameObject;

  // accessors -------------------------------------------------------

  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // repeated .protos.Event event = 2;
  inline int event_size() const;
  inline void clear_event();
  static const int kEventFieldNumber = 2;
  inline const ::protos::Event& event(int index) const;
  inline ::protos::Event* mutable_event(int index);
  inline ::protos::Event* add_event();
  inline const ::google::protobuf::RepeatedPtrField< ::protos::Event >&
      event() const;
  inline ::google::protobuf::RepeatedPtrField< ::protos::Event >*
      mutable_event();

  // repeated .protos.Message.GameObject gameObject = 3;
  inline int gameobject_size() const;
  inline void clear_gameobject();
  static const int kGameObjectFieldNumber = 3;
  inline const ::protos::Message_GameObject& gameobject(int index) const;
  inline ::protos::Message_GameObject* mutable_gameobject(int index);
  inline ::protos::Message_GameObject* add_gameobject();
  inline const ::google::protobuf::RepeatedPtrField< ::protos::Message_GameObject >&
      gameobject() const;
  inline ::google::protobuf::RepeatedPtrField< ::protos::Message_GameObject >*
      mutable_gameobject();

  // @@protoc_insertion_point(class_scope:protos.Message)
 private:
  inline void set_has_id();
  inline void clear_has_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::protos::Event > event_;
  ::google::protobuf::RepeatedPtrField< ::protos::Message_GameObject > gameobject_;
  ::google::protobuf::int32 id_;
  friend void  protobuf_AddDesc_Protos_2eproto();
  friend void protobuf_AssignDesc_Protos_2eproto();
  friend void protobuf_ShutdownFile_Protos_2eproto();

  void InitAsDefaultInstance();
  static Message* default_instance_;
};
// ===================================================================


// ===================================================================

// Event

// optional int32 id = 1;
inline bool Event::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Event::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Event::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Event::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Event::id() const {
  // @@protoc_insertion_point(field_get:protos.Event.id)
  return id_;
}
inline void Event::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:protos.Event.id)
}

// optional .protos.Event.Type type = 2;
inline bool Event::has_type() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Event::set_has_type() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Event::clear_has_type() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Event::clear_type() {
  type_ = 1;
  clear_has_type();
}
inline ::protos::Event_Type Event::type() const {
  // @@protoc_insertion_point(field_get:protos.Event.type)
  return static_cast< ::protos::Event_Type >(type_);
}
inline void Event::set_type(::protos::Event_Type value) {
  assert(::protos::Event_Type_IsValid(value));
  set_has_type();
  type_ = value;
  // @@protoc_insertion_point(field_set:protos.Event.type)
}

// optional int32 clientID = 6;
inline bool Event::has_clientid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Event::set_has_clientid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Event::clear_has_clientid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Event::clear_clientid() {
  clientid_ = 0;
  clear_has_clientid();
}
inline ::google::protobuf::int32 Event::clientid() const {
  // @@protoc_insertion_point(field_get:protos.Event.clientID)
  return clientid_;
}
inline void Event::set_clientid(::google::protobuf::int32 value) {
  set_has_clientid();
  clientid_ = value;
  // @@protoc_insertion_point(field_set:protos.Event.clientID)
}

// optional .protos.Event.Direction direction = 7;
inline bool Event::has_direction() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Event::set_has_direction() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Event::clear_has_direction() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Event::clear_direction() {
  direction_ = 1;
  clear_has_direction();
}
inline ::protos::Event_Direction Event::direction() const {
  // @@protoc_insertion_point(field_get:protos.Event.direction)
  return static_cast< ::protos::Event_Direction >(direction_);
}
inline void Event::set_direction(::protos::Event_Direction value) {
  assert(::protos::Event_Direction_IsValid(value));
  set_has_direction();
  direction_ = value;
  // @@protoc_insertion_point(field_set:protos.Event.direction)
}

// -------------------------------------------------------------------

// Message_GameObject

// optional int32 id = 1;
inline bool Message_GameObject::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Message_GameObject::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Message_GameObject::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Message_GameObject::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Message_GameObject::id() const {
  // @@protoc_insertion_point(field_get:protos.Message.GameObject.id)
  return id_;
}
inline void Message_GameObject::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:protos.Message.GameObject.id)
}

// repeated double matrix = 2;
inline int Message_GameObject::matrix_size() const {
  return matrix_.size();
}
inline void Message_GameObject::clear_matrix() {
  matrix_.Clear();
}
inline double Message_GameObject::matrix(int index) const {
  // @@protoc_insertion_point(field_get:protos.Message.GameObject.matrix)
  return matrix_.Get(index);
}
inline void Message_GameObject::set_matrix(int index, double value) {
  matrix_.Set(index, value);
  // @@protoc_insertion_point(field_set:protos.Message.GameObject.matrix)
}
inline void Message_GameObject::add_matrix(double value) {
  matrix_.Add(value);
  // @@protoc_insertion_point(field_add:protos.Message.GameObject.matrix)
}
inline const ::google::protobuf::RepeatedField< double >&
Message_GameObject::matrix() const {
  // @@protoc_insertion_point(field_list:protos.Message.GameObject.matrix)
  return matrix_;
}
inline ::google::protobuf::RepeatedField< double >*
Message_GameObject::mutable_matrix() {
  // @@protoc_insertion_point(field_mutable_list:protos.Message.GameObject.matrix)
  return &matrix_;
}

// -------------------------------------------------------------------

// Message

// optional int32 id = 1;
inline bool Message::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Message::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Message::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Message::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Message::id() const {
  // @@protoc_insertion_point(field_get:protos.Message.id)
  return id_;
}
inline void Message::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:protos.Message.id)
}

// repeated .protos.Event event = 2;
inline int Message::event_size() const {
  return event_.size();
}
inline void Message::clear_event() {
  event_.Clear();
}
inline const ::protos::Event& Message::event(int index) const {
  // @@protoc_insertion_point(field_get:protos.Message.event)
  return event_.Get(index);
}
inline ::protos::Event* Message::mutable_event(int index) {
  // @@protoc_insertion_point(field_mutable:protos.Message.event)
  return event_.Mutable(index);
}
inline ::protos::Event* Message::add_event() {
  // @@protoc_insertion_point(field_add:protos.Message.event)
  return event_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::protos::Event >&
Message::event() const {
  // @@protoc_insertion_point(field_list:protos.Message.event)
  return event_;
}
inline ::google::protobuf::RepeatedPtrField< ::protos::Event >*
Message::mutable_event() {
  // @@protoc_insertion_point(field_mutable_list:protos.Message.event)
  return &event_;
}

// repeated .protos.Message.GameObject gameObject = 3;
inline int Message::gameobject_size() const {
  return gameobject_.size();
}
inline void Message::clear_gameobject() {
  gameobject_.Clear();
}
inline const ::protos::Message_GameObject& Message::gameobject(int index) const {
  // @@protoc_insertion_point(field_get:protos.Message.gameObject)
  return gameobject_.Get(index);
}
inline ::protos::Message_GameObject* Message::mutable_gameobject(int index) {
  // @@protoc_insertion_point(field_mutable:protos.Message.gameObject)
  return gameobject_.Mutable(index);
}
inline ::protos::Message_GameObject* Message::add_gameobject() {
  // @@protoc_insertion_point(field_add:protos.Message.gameObject)
  return gameobject_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::protos::Message_GameObject >&
Message::gameobject() const {
  // @@protoc_insertion_point(field_list:protos.Message.gameObject)
  return gameobject_;
}
inline ::google::protobuf::RepeatedPtrField< ::protos::Message_GameObject >*
Message::mutable_gameobject() {
  // @@protoc_insertion_point(field_mutable_list:protos.Message.gameObject)
  return &gameobject_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace protos

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::protos::Event_Type> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protos::Event_Type>() {
  return ::protos::Event_Type_descriptor();
}
template <> struct is_proto_enum< ::protos::Event_Direction> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::protos::Event_Direction>() {
  return ::protos::Event_Direction_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Protos_2eproto__INCLUDED
