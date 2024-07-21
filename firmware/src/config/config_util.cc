#include "config_util.h"

#include "proto/config.pb.h"

#define CASE_OVERRIDE_DEFAULT_IMPL(PROTO_T, ACC_NAME, CPP_T)                 \
  case decaproto::FieldType::k##PROTO_T:                                     \
    if (field.IsRepeated()) {                                                \
      size_t src_size = reflection->FieldSize(&src, field.GetFieldNumber()); \
      for (size_t i = 0; i < src_size; i++) {                                \
        if (reflection->FieldSize(&dst, field.GetFieldNumber()) <= i) {      \
          *reflection->AddRepeated##ACC_NAME(&dst, field.GetFieldNumber()) = \
              reflection->GetRepeated##ACC_NAME(                             \
                  &src, field.GetFieldNumber(), i);                          \
        } else {                                                             \
          if (reflection->GetRepeated##ACC_NAME(                             \
                  &dst, field.GetFieldNumber(), i) == CPP_T()) {             \
            reflection->SetRepeated##ACC_NAME(                               \
                &dst,                                                        \
                field.GetFieldNumber(),                                      \
                i,                                                           \
                reflection->Get##ACC_NAME(&src, field.GetFieldNumber()));    \
          }                                                                  \
        }                                                                    \
      }                                                                      \
    } else {                                                                 \
      if (reflection->Get##ACC_NAME(&dst, field.GetFieldNumber()) ==         \
          CPP_T()) {                                                         \
        reflection->Set##ACC_NAME(                                           \
            &dst,                                                            \
            field.GetFieldNumber(),                                          \
            reflection->Get##ACC_NAME(&src, field.GetFieldNumber()));        \
      }                                                                      \
    }                                                                        \
    break;

#define CASE_OVERRIDE_DEFAULT(PROTO_T, CPP_T) \
  CASE_OVERRIDE_DEFAULT_IMPL(PROTO_T, PROTO_T, CPP_T)

void ComplementField(
    const decaproto::Message& src,
    decaproto::Message& dst,
    const decaproto::FieldDescriptor& field) {
  const decaproto::Reflection* reflection = src.GetReflection();
  switch (field.GetType()) {
    CASE_OVERRIDE_DEFAULT(Uint32, uint32_t);
    CASE_OVERRIDE_DEFAULT(Uint64, uint64_t);
    CASE_OVERRIDE_DEFAULT(Int32, int32_t);
    CASE_OVERRIDE_DEFAULT(Int64, int64_t);
    CASE_OVERRIDE_DEFAULT(Double, double);
    CASE_OVERRIDE_DEFAULT(Float, float);
    CASE_OVERRIDE_DEFAULT(String, std::string);
    CASE_OVERRIDE_DEFAULT_IMPL(Enum, EnumValue, int);
    case decaproto::FieldType::kMessage: {
      if (field.IsRepeated()) {
        size_t src_size = reflection->FieldSize(&src, field.GetFieldNumber());
        for (size_t i = 0; i < src_size; i++) {
          decaproto::Message* mut_msg;
          if (reflection->FieldSize(&dst, field.GetFieldNumber()) <= i) {
            mut_msg =
                reflection->AddRepeatedMessage(&dst, field.GetFieldNumber());
          } else {
            mut_msg = reflection->MutableRepeatedMessage(
                &dst, field.GetFieldNumber(), i);
          }
          ComplementMessage(
              reflection->GetRepeatedMessage(&src, field.GetFieldNumber(), i),
              *mut_msg);
        }
      } else {
        if (reflection->HasField(&src, field.GetFieldNumber())) {
          // If src has the sub message field, complement the dst message with
          // it.
          decaproto::Message* mut_msg =
              reflection->MutableMessage(&dst, field.GetFieldNumber());
          ComplementMessage(
              reflection->GetMessage(&src, field.GetFieldNumber()), *mut_msg);
        }
      }
      break;
    }
    default:
      Serial.printf(
          "ERROR: ComplementField: Unsupported field type. FieldType "
          "%d\n",
          field.GetType());
  }
}

// Fill fields with the default value in the dst message with the value in
// the src message. The am
void ComplementMessage(const decaproto::Message& src, decaproto::Message& dst) {
  for (const decaproto::FieldDescriptor& field :
       src.GetDescriptor()->GetFields()) {
    ComplementField(src, dst, field);
  }
}
