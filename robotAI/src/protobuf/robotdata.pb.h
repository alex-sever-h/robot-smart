// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: robotdata.proto

#ifndef PROTOBUF_robotdata_2eproto__INCLUDED
#define PROTOBUF_robotdata_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace robotdata {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_robotdata_2eproto();
void protobuf_AssignDesc_robotdata_2eproto();
void protobuf_ShutdownFile_robotdata_2eproto();

class RobotInfo;
class SensorInfo;
class MapParticle;
class FullMap;
class PathDot;
class FullPath;

// ===================================================================

class RobotInfo : public ::google::protobuf::Message {
 public:
  RobotInfo();
  virtual ~RobotInfo();

  RobotInfo(const RobotInfo& from);

  inline RobotInfo& operator=(const RobotInfo& from) {
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
  static const RobotInfo& default_instance();

  void Swap(RobotInfo* other);

  // implements Message ----------------------------------------------

  RobotInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RobotInfo& from);
  void MergeFrom(const RobotInfo& from);
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

  // required int32 length = 1;
  inline bool has_length() const;
  inline void clear_length();
  static const int kLengthFieldNumber = 1;
  inline ::google::protobuf::int32 length() const;
  inline void set_length(::google::protobuf::int32 value);

  // required int32 width = 2;
  inline bool has_width() const;
  inline void clear_width();
  static const int kWidthFieldNumber = 2;
  inline ::google::protobuf::int32 width() const;
  inline void set_width(::google::protobuf::int32 value);

  // required float posx = 3;
  inline bool has_posx() const;
  inline void clear_posx();
  static const int kPosxFieldNumber = 3;
  inline float posx() const;
  inline void set_posx(float value);

  // required float posy = 4;
  inline bool has_posy() const;
  inline void clear_posy();
  static const int kPosyFieldNumber = 4;
  inline float posy() const;
  inline void set_posy(float value);

  // required float theta = 5;
  inline bool has_theta() const;
  inline void clear_theta();
  static const int kThetaFieldNumber = 5;
  inline float theta() const;
  inline void set_theta(float value);

  // @@protoc_insertion_point(class_scope:robotdata.RobotInfo)
 private:
  inline void set_has_length();
  inline void clear_has_length();
  inline void set_has_width();
  inline void clear_has_width();
  inline void set_has_posx();
  inline void clear_has_posx();
  inline void set_has_posy();
  inline void clear_has_posy();
  inline void set_has_theta();
  inline void clear_has_theta();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 length_;
  ::google::protobuf::int32 width_;
  float posx_;
  float posy_;
  float theta_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();

  void InitAsDefaultInstance();
  static RobotInfo* default_instance_;
};
// -------------------------------------------------------------------

class SensorInfo : public ::google::protobuf::Message {
 public:
  SensorInfo();
  virtual ~SensorInfo();

  SensorInfo(const SensorInfo& from);

  inline SensorInfo& operator=(const SensorInfo& from) {
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
  static const SensorInfo& default_instance();

  void Swap(SensorInfo* other);

  // implements Message ----------------------------------------------

  SensorInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SensorInfo& from);
  void MergeFrom(const SensorInfo& from);
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

  // required float angleSpan = 1;
  inline bool has_anglespan() const;
  inline void clear_anglespan();
  static const int kAngleSpanFieldNumber = 1;
  inline float anglespan() const;
  inline void set_anglespan(float value);

  // required float angleCenter = 2;
  inline bool has_anglecenter() const;
  inline void clear_anglecenter();
  static const int kAngleCenterFieldNumber = 2;
  inline float anglecenter() const;
  inline void set_anglecenter(float value);

  // required float offsetXmapMM = 3;
  inline bool has_offsetxmapmm() const;
  inline void clear_offsetxmapmm();
  static const int kOffsetXmapMMFieldNumber = 3;
  inline float offsetxmapmm() const;
  inline void set_offsetxmapmm(float value);

  // required float offsetYmapMM = 4;
  inline bool has_offsetymapmm() const;
  inline void clear_offsetymapmm();
  static const int kOffsetYmapMMFieldNumber = 4;
  inline float offsetymapmm() const;
  inline void set_offsetymapmm(float value);

  // required int32 distance = 5;
  inline bool has_distance() const;
  inline void clear_distance();
  static const int kDistanceFieldNumber = 5;
  inline ::google::protobuf::int32 distance() const;
  inline void set_distance(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:robotdata.SensorInfo)
 private:
  inline void set_has_anglespan();
  inline void clear_has_anglespan();
  inline void set_has_anglecenter();
  inline void clear_has_anglecenter();
  inline void set_has_offsetxmapmm();
  inline void clear_has_offsetxmapmm();
  inline void set_has_offsetymapmm();
  inline void clear_has_offsetymapmm();
  inline void set_has_distance();
  inline void clear_has_distance();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  float anglespan_;
  float anglecenter_;
  float offsetxmapmm_;
  float offsetymapmm_;
  ::google::protobuf::int32 distance_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();

  void InitAsDefaultInstance();
  static SensorInfo* default_instance_;
};
// -------------------------------------------------------------------

class MapParticle : public ::google::protobuf::Message {
 public:
  MapParticle();
  virtual ~MapParticle();

  MapParticle(const MapParticle& from);

  inline MapParticle& operator=(const MapParticle& from) {
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
  static const MapParticle& default_instance();

  void Swap(MapParticle* other);

  // implements Message ----------------------------------------------

  MapParticle* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const MapParticle& from);
  void MergeFrom(const MapParticle& from);
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

  // required float posx = 1;
  inline bool has_posx() const;
  inline void clear_posx();
  static const int kPosxFieldNumber = 1;
  inline float posx() const;
  inline void set_posx(float value);

  // required float posy = 2;
  inline bool has_posy() const;
  inline void clear_posy();
  static const int kPosyFieldNumber = 2;
  inline float posy() const;
  inline void set_posy(float value);

  // required float weight = 3;
  inline bool has_weight() const;
  inline void clear_weight();
  static const int kWeightFieldNumber = 3;
  inline float weight() const;
  inline void set_weight(float value);

  // @@protoc_insertion_point(class_scope:robotdata.MapParticle)
 private:
  inline void set_has_posx();
  inline void clear_has_posx();
  inline void set_has_posy();
  inline void clear_has_posy();
  inline void set_has_weight();
  inline void clear_has_weight();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  float posx_;
  float posy_;
  float weight_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();

  void InitAsDefaultInstance();
  static MapParticle* default_instance_;
};
// -------------------------------------------------------------------

class FullMap : public ::google::protobuf::Message {
 public:
  FullMap();
  virtual ~FullMap();

  FullMap(const FullMap& from);

  inline FullMap& operator=(const FullMap& from) {
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
  static const FullMap& default_instance();

  void Swap(FullMap* other);

  // implements Message ----------------------------------------------

  FullMap* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FullMap& from);
  void MergeFrom(const FullMap& from);
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

  // repeated .robotdata.MapParticle wallParticles = 1;
  inline int wallparticles_size() const;
  inline void clear_wallparticles();
  static const int kWallParticlesFieldNumber = 1;
  inline const ::robotdata::MapParticle& wallparticles(int index) const;
  inline ::robotdata::MapParticle* mutable_wallparticles(int index);
  inline ::robotdata::MapParticle* add_wallparticles();
  inline const ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle >&
      wallparticles() const;
  inline ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle >*
      mutable_wallparticles();

  // repeated .robotdata.MapParticle safeParticles = 2;
  inline int safeparticles_size() const;
  inline void clear_safeparticles();
  static const int kSafeParticlesFieldNumber = 2;
  inline const ::robotdata::MapParticle& safeparticles(int index) const;
  inline ::robotdata::MapParticle* mutable_safeparticles(int index);
  inline ::robotdata::MapParticle* add_safeparticles();
  inline const ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle >&
      safeparticles() const;
  inline ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle >*
      mutable_safeparticles();

  // @@protoc_insertion_point(class_scope:robotdata.FullMap)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle > wallparticles_;
  ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle > safeparticles_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();

  void InitAsDefaultInstance();
  static FullMap* default_instance_;
};
// -------------------------------------------------------------------

class PathDot : public ::google::protobuf::Message {
 public:
  PathDot();
  virtual ~PathDot();

  PathDot(const PathDot& from);

  inline PathDot& operator=(const PathDot& from) {
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
  static const PathDot& default_instance();

  void Swap(PathDot* other);

  // implements Message ----------------------------------------------

  PathDot* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const PathDot& from);
  void MergeFrom(const PathDot& from);
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

  // required float x = 1;
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 1;
  inline float x() const;
  inline void set_x(float value);

  // required float y = 2;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 2;
  inline float y() const;
  inline void set_y(float value);

  // @@protoc_insertion_point(class_scope:robotdata.PathDot)
 private:
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  float x_;
  float y_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();

  void InitAsDefaultInstance();
  static PathDot* default_instance_;
};
// -------------------------------------------------------------------

class FullPath : public ::google::protobuf::Message {
 public:
  FullPath();
  virtual ~FullPath();

  FullPath(const FullPath& from);

  inline FullPath& operator=(const FullPath& from) {
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
  static const FullPath& default_instance();

  void Swap(FullPath* other);

  // implements Message ----------------------------------------------

  FullPath* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const FullPath& from);
  void MergeFrom(const FullPath& from);
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

  // repeated .robotdata.PathDot path = 4;
  inline int path_size() const;
  inline void clear_path();
  static const int kPathFieldNumber = 4;
  inline const ::robotdata::PathDot& path(int index) const;
  inline ::robotdata::PathDot* mutable_path(int index);
  inline ::robotdata::PathDot* add_path();
  inline const ::google::protobuf::RepeatedPtrField< ::robotdata::PathDot >&
      path() const;
  inline ::google::protobuf::RepeatedPtrField< ::robotdata::PathDot >*
      mutable_path();

  // @@protoc_insertion_point(class_scope:robotdata.FullPath)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::robotdata::PathDot > path_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_robotdata_2eproto();
  friend void protobuf_AssignDesc_robotdata_2eproto();
  friend void protobuf_ShutdownFile_robotdata_2eproto();

  void InitAsDefaultInstance();
  static FullPath* default_instance_;
};
// ===================================================================


// ===================================================================

// RobotInfo

// required int32 length = 1;
inline bool RobotInfo::has_length() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RobotInfo::set_has_length() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RobotInfo::clear_has_length() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RobotInfo::clear_length() {
  length_ = 0;
  clear_has_length();
}
inline ::google::protobuf::int32 RobotInfo::length() const {
  return length_;
}
inline void RobotInfo::set_length(::google::protobuf::int32 value) {
  set_has_length();
  length_ = value;
}

// required int32 width = 2;
inline bool RobotInfo::has_width() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RobotInfo::set_has_width() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RobotInfo::clear_has_width() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RobotInfo::clear_width() {
  width_ = 0;
  clear_has_width();
}
inline ::google::protobuf::int32 RobotInfo::width() const {
  return width_;
}
inline void RobotInfo::set_width(::google::protobuf::int32 value) {
  set_has_width();
  width_ = value;
}

// required float posx = 3;
inline bool RobotInfo::has_posx() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RobotInfo::set_has_posx() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RobotInfo::clear_has_posx() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RobotInfo::clear_posx() {
  posx_ = 0;
  clear_has_posx();
}
inline float RobotInfo::posx() const {
  return posx_;
}
inline void RobotInfo::set_posx(float value) {
  set_has_posx();
  posx_ = value;
}

// required float posy = 4;
inline bool RobotInfo::has_posy() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void RobotInfo::set_has_posy() {
  _has_bits_[0] |= 0x00000008u;
}
inline void RobotInfo::clear_has_posy() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void RobotInfo::clear_posy() {
  posy_ = 0;
  clear_has_posy();
}
inline float RobotInfo::posy() const {
  return posy_;
}
inline void RobotInfo::set_posy(float value) {
  set_has_posy();
  posy_ = value;
}

// required float theta = 5;
inline bool RobotInfo::has_theta() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void RobotInfo::set_has_theta() {
  _has_bits_[0] |= 0x00000010u;
}
inline void RobotInfo::clear_has_theta() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void RobotInfo::clear_theta() {
  theta_ = 0;
  clear_has_theta();
}
inline float RobotInfo::theta() const {
  return theta_;
}
inline void RobotInfo::set_theta(float value) {
  set_has_theta();
  theta_ = value;
}

// -------------------------------------------------------------------

// SensorInfo

// required float angleSpan = 1;
inline bool SensorInfo::has_anglespan() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SensorInfo::set_has_anglespan() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SensorInfo::clear_has_anglespan() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SensorInfo::clear_anglespan() {
  anglespan_ = 0;
  clear_has_anglespan();
}
inline float SensorInfo::anglespan() const {
  return anglespan_;
}
inline void SensorInfo::set_anglespan(float value) {
  set_has_anglespan();
  anglespan_ = value;
}

// required float angleCenter = 2;
inline bool SensorInfo::has_anglecenter() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SensorInfo::set_has_anglecenter() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SensorInfo::clear_has_anglecenter() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SensorInfo::clear_anglecenter() {
  anglecenter_ = 0;
  clear_has_anglecenter();
}
inline float SensorInfo::anglecenter() const {
  return anglecenter_;
}
inline void SensorInfo::set_anglecenter(float value) {
  set_has_anglecenter();
  anglecenter_ = value;
}

// required float offsetXmapMM = 3;
inline bool SensorInfo::has_offsetxmapmm() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void SensorInfo::set_has_offsetxmapmm() {
  _has_bits_[0] |= 0x00000004u;
}
inline void SensorInfo::clear_has_offsetxmapmm() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void SensorInfo::clear_offsetxmapmm() {
  offsetxmapmm_ = 0;
  clear_has_offsetxmapmm();
}
inline float SensorInfo::offsetxmapmm() const {
  return offsetxmapmm_;
}
inline void SensorInfo::set_offsetxmapmm(float value) {
  set_has_offsetxmapmm();
  offsetxmapmm_ = value;
}

// required float offsetYmapMM = 4;
inline bool SensorInfo::has_offsetymapmm() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void SensorInfo::set_has_offsetymapmm() {
  _has_bits_[0] |= 0x00000008u;
}
inline void SensorInfo::clear_has_offsetymapmm() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void SensorInfo::clear_offsetymapmm() {
  offsetymapmm_ = 0;
  clear_has_offsetymapmm();
}
inline float SensorInfo::offsetymapmm() const {
  return offsetymapmm_;
}
inline void SensorInfo::set_offsetymapmm(float value) {
  set_has_offsetymapmm();
  offsetymapmm_ = value;
}

// required int32 distance = 5;
inline bool SensorInfo::has_distance() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void SensorInfo::set_has_distance() {
  _has_bits_[0] |= 0x00000010u;
}
inline void SensorInfo::clear_has_distance() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void SensorInfo::clear_distance() {
  distance_ = 0;
  clear_has_distance();
}
inline ::google::protobuf::int32 SensorInfo::distance() const {
  return distance_;
}
inline void SensorInfo::set_distance(::google::protobuf::int32 value) {
  set_has_distance();
  distance_ = value;
}

// -------------------------------------------------------------------

// MapParticle

// required float posx = 1;
inline bool MapParticle::has_posx() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void MapParticle::set_has_posx() {
  _has_bits_[0] |= 0x00000001u;
}
inline void MapParticle::clear_has_posx() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void MapParticle::clear_posx() {
  posx_ = 0;
  clear_has_posx();
}
inline float MapParticle::posx() const {
  return posx_;
}
inline void MapParticle::set_posx(float value) {
  set_has_posx();
  posx_ = value;
}

// required float posy = 2;
inline bool MapParticle::has_posy() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void MapParticle::set_has_posy() {
  _has_bits_[0] |= 0x00000002u;
}
inline void MapParticle::clear_has_posy() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void MapParticle::clear_posy() {
  posy_ = 0;
  clear_has_posy();
}
inline float MapParticle::posy() const {
  return posy_;
}
inline void MapParticle::set_posy(float value) {
  set_has_posy();
  posy_ = value;
}

// required float weight = 3;
inline bool MapParticle::has_weight() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void MapParticle::set_has_weight() {
  _has_bits_[0] |= 0x00000004u;
}
inline void MapParticle::clear_has_weight() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void MapParticle::clear_weight() {
  weight_ = 0;
  clear_has_weight();
}
inline float MapParticle::weight() const {
  return weight_;
}
inline void MapParticle::set_weight(float value) {
  set_has_weight();
  weight_ = value;
}

// -------------------------------------------------------------------

// FullMap

// repeated .robotdata.MapParticle wallParticles = 1;
inline int FullMap::wallparticles_size() const {
  return wallparticles_.size();
}
inline void FullMap::clear_wallparticles() {
  wallparticles_.Clear();
}
inline const ::robotdata::MapParticle& FullMap::wallparticles(int index) const {
  return wallparticles_.Get(index);
}
inline ::robotdata::MapParticle* FullMap::mutable_wallparticles(int index) {
  return wallparticles_.Mutable(index);
}
inline ::robotdata::MapParticle* FullMap::add_wallparticles() {
  return wallparticles_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle >&
FullMap::wallparticles() const {
  return wallparticles_;
}
inline ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle >*
FullMap::mutable_wallparticles() {
  return &wallparticles_;
}

// repeated .robotdata.MapParticle safeParticles = 2;
inline int FullMap::safeparticles_size() const {
  return safeparticles_.size();
}
inline void FullMap::clear_safeparticles() {
  safeparticles_.Clear();
}
inline const ::robotdata::MapParticle& FullMap::safeparticles(int index) const {
  return safeparticles_.Get(index);
}
inline ::robotdata::MapParticle* FullMap::mutable_safeparticles(int index) {
  return safeparticles_.Mutable(index);
}
inline ::robotdata::MapParticle* FullMap::add_safeparticles() {
  return safeparticles_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle >&
FullMap::safeparticles() const {
  return safeparticles_;
}
inline ::google::protobuf::RepeatedPtrField< ::robotdata::MapParticle >*
FullMap::mutable_safeparticles() {
  return &safeparticles_;
}

// -------------------------------------------------------------------

// PathDot

// required float x = 1;
inline bool PathDot::has_x() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PathDot::set_has_x() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PathDot::clear_has_x() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PathDot::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline float PathDot::x() const {
  return x_;
}
inline void PathDot::set_x(float value) {
  set_has_x();
  x_ = value;
}

// required float y = 2;
inline bool PathDot::has_y() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PathDot::set_has_y() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PathDot::clear_has_y() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PathDot::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline float PathDot::y() const {
  return y_;
}
inline void PathDot::set_y(float value) {
  set_has_y();
  y_ = value;
}

// -------------------------------------------------------------------

// FullPath

// repeated .robotdata.PathDot path = 4;
inline int FullPath::path_size() const {
  return path_.size();
}
inline void FullPath::clear_path() {
  path_.Clear();
}
inline const ::robotdata::PathDot& FullPath::path(int index) const {
  return path_.Get(index);
}
inline ::robotdata::PathDot* FullPath::mutable_path(int index) {
  return path_.Mutable(index);
}
inline ::robotdata::PathDot* FullPath::add_path() {
  return path_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::robotdata::PathDot >&
FullPath::path() const {
  return path_;
}
inline ::google::protobuf::RepeatedPtrField< ::robotdata::PathDot >*
FullPath::mutable_path() {
  return &path_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace robotdata

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_robotdata_2eproto__INCLUDED
