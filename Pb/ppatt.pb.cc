// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ppatt.proto

#include "ppatt.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace org {
namespace infinity {
namespace idm {
constexpr ParamPattern::ParamPattern(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : points_()
  , startbeat_(0u)
  , starttick_(0u)
  , length_(uint64_t{0u}){}
struct ParamPatternDefaultTypeInternal {
  constexpr ParamPatternDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~ParamPatternDefaultTypeInternal() {}
  union {
    ParamPattern _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT ParamPatternDefaultTypeInternal _ParamPattern_default_instance_;
}  // namespace idm
}  // namespace infinity
}  // namespace org
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_ppatt_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_ppatt_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_ppatt_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_ppatt_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::org::infinity::idm::ParamPattern, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::org::infinity::idm::ParamPattern, startbeat_),
  PROTOBUF_FIELD_OFFSET(::org::infinity::idm::ParamPattern, starttick_),
  PROTOBUF_FIELD_OFFSET(::org::infinity::idm::ParamPattern, length_),
  PROTOBUF_FIELD_OFFSET(::org::infinity::idm::ParamPattern, points_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::org::infinity::idm::ParamPattern)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::org::infinity::idm::_ParamPattern_default_instance_),
};

const char descriptor_table_protodef_ppatt_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013ppatt.proto\022\020org.infinity.idm\032\013point.p"
  "roto\"m\n\014ParamPattern\022\021\n\tstartBeat\030\001 \001(\r\022"
  "\021\n\tstartTick\030\002 \001(\r\022\016\n\006length\030\003 \001(\004\022\'\n\006po"
  "ints\030\004 \003(\0132\027.org.infinity.idm.Pointb\006pro"
  "to3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_ppatt_2eproto_deps[1] = {
  &::descriptor_table_point_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_ppatt_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ppatt_2eproto = {
  false, false, 163, descriptor_table_protodef_ppatt_2eproto, "ppatt.proto", 
  &descriptor_table_ppatt_2eproto_once, descriptor_table_ppatt_2eproto_deps, 1, 1,
  schemas, file_default_instances, TableStruct_ppatt_2eproto::offsets,
  file_level_metadata_ppatt_2eproto, file_level_enum_descriptors_ppatt_2eproto, file_level_service_descriptors_ppatt_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_ppatt_2eproto_getter() {
  return &descriptor_table_ppatt_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_ppatt_2eproto(&descriptor_table_ppatt_2eproto);
namespace org {
namespace infinity {
namespace idm {

// ===================================================================

class ParamPattern::_Internal {
 public:
};

void ParamPattern::clear_points() {
  points_.Clear();
}
ParamPattern::ParamPattern(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned),
  points_(arena) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:org.infinity.idm.ParamPattern)
}
ParamPattern::ParamPattern(const ParamPattern& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      points_(from.points_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&startbeat_, &from.startbeat_,
    static_cast<size_t>(reinterpret_cast<char*>(&length_) -
    reinterpret_cast<char*>(&startbeat_)) + sizeof(length_));
  // @@protoc_insertion_point(copy_constructor:org.infinity.idm.ParamPattern)
}

inline void ParamPattern::SharedCtor() {
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&startbeat_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&length_) -
    reinterpret_cast<char*>(&startbeat_)) + sizeof(length_));
}

ParamPattern::~ParamPattern() {
  // @@protoc_insertion_point(destructor:org.infinity.idm.ParamPattern)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void ParamPattern::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void ParamPattern::ArenaDtor(void* object) {
  ParamPattern* _this = reinterpret_cast< ParamPattern* >(object);
  (void)_this;
}
void ParamPattern::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void ParamPattern::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void ParamPattern::Clear() {
// @@protoc_insertion_point(message_clear_start:org.infinity.idm.ParamPattern)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  points_.Clear();
  ::memset(&startbeat_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&length_) -
      reinterpret_cast<char*>(&startbeat_)) + sizeof(length_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ParamPattern::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // uint32 startBeat = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          startbeat_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 startTick = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          starttick_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint64 length = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          length_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated .org.infinity.idm.Point points = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_points(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<34>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag == 0) || ((tag & 7) == 4)) {
          CHK_(ptr);
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* ParamPattern::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:org.infinity.idm.ParamPattern)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 startBeat = 1;
  if (this->_internal_startbeat() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(1, this->_internal_startbeat(), target);
  }

  // uint32 startTick = 2;
  if (this->_internal_starttick() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(2, this->_internal_starttick(), target);
  }

  // uint64 length = 3;
  if (this->_internal_length() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(3, this->_internal_length(), target);
  }

  // repeated .org.infinity.idm.Point points = 4;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_points_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(4, this->_internal_points(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:org.infinity.idm.ParamPattern)
  return target;
}

size_t ParamPattern::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:org.infinity.idm.ParamPattern)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .org.infinity.idm.Point points = 4;
  total_size += 1UL * this->_internal_points_size();
  for (const auto& msg : this->points_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // uint32 startBeat = 1;
  if (this->_internal_startbeat() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_startbeat());
  }

  // uint32 startTick = 2;
  if (this->_internal_starttick() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_starttick());
  }

  // uint64 length = 3;
  if (this->_internal_length() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_length());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ParamPattern::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    ParamPattern::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ParamPattern::GetClassData() const { return &_class_data_; }

void ParamPattern::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message*to,
                      const ::PROTOBUF_NAMESPACE_ID::Message&from) {
  static_cast<ParamPattern *>(to)->MergeFrom(
      static_cast<const ParamPattern &>(from));
}


void ParamPattern::MergeFrom(const ParamPattern& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:org.infinity.idm.ParamPattern)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  points_.MergeFrom(from.points_);
  if (from._internal_startbeat() != 0) {
    _internal_set_startbeat(from._internal_startbeat());
  }
  if (from._internal_starttick() != 0) {
    _internal_set_starttick(from._internal_starttick());
  }
  if (from._internal_length() != 0) {
    _internal_set_length(from._internal_length());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ParamPattern::CopyFrom(const ParamPattern& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:org.infinity.idm.ParamPattern)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ParamPattern::IsInitialized() const {
  return true;
}

void ParamPattern::InternalSwap(ParamPattern* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  points_.InternalSwap(&other->points_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(ParamPattern, length_)
      + sizeof(ParamPattern::length_)
      - PROTOBUF_FIELD_OFFSET(ParamPattern, startbeat_)>(
          reinterpret_cast<char*>(&startbeat_),
          reinterpret_cast<char*>(&other->startbeat_));
}

::PROTOBUF_NAMESPACE_ID::Metadata ParamPattern::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_ppatt_2eproto_getter, &descriptor_table_ppatt_2eproto_once,
      file_level_metadata_ppatt_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace idm
}  // namespace infinity
}  // namespace org
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::org::infinity::idm::ParamPattern* Arena::CreateMaybeMessage< ::org::infinity::idm::ParamPattern >(Arena* arena) {
  return Arena::CreateMessageInternal< ::org::infinity::idm::ParamPattern >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>