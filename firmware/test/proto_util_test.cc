#include "proto_util.h"

#include "proto/config.pb.h"
#include "unity.h"

void setUp(void) {
}

void tearDown(void) {
}

void testComplementField_repeated_field(void) {
  UdongConfig default_config;

  default_config.mutable_d_pad_config()->set_lr_socd_mode(
      DPadConfig::LRSocdCleanerMode::LR_LAST_PRIORITY);
  default_config.mutable_d_pad_config()->set_ud_socd_mode(
      DPadConfig::UDSocdCleanerMode::UP_PRIORITY);

  AnalogSwitchGroup *def_group0 = default_config.add_analog_switch_groups();
  def_group0->set_analog_switch_group_id(0);
  def_group0->set_total_travel_distance(3.5);
  def_group0->mutable_rapid_trigger()->set_act(1.2);
  AnalogSwitchGroup *def_group1 = default_config.add_analog_switch_groups();
  def_group1->set_analog_switch_group_id(1);
  def_group1->set_total_travel_distance(4.1);
  def_group1->mutable_rapid_trigger()->set_act(0.6);
  AnalogSwitchGroup *def_group2 = default_config.add_analog_switch_groups();
  def_group2->set_analog_switch_group_id(2);
  def_group2->set_total_travel_distance(4.5);
  def_group2->mutable_rapid_trigger()->set_act(0.3);

  UdongConfig dst;
  AnalogSwitchGroup *dst_group0 = dst.add_analog_switch_groups();
  dst_group0->set_analog_switch_group_id(0);
  dst_group0->mutable_rapid_trigger()->set_act(1.0);

  ComplementMessage(default_config, dst);

  TEST_ASSERT_EQUAL(
      DPadConfig::LRSocdCleanerMode::LR_LAST_PRIORITY,
      dst.d_pad_config().lr_socd_mode());
  TEST_ASSERT_EQUAL(
      DPadConfig::UDSocdCleanerMode::UP_PRIORITY,
      dst.d_pad_config().ud_socd_mode());

  TEST_ASSERT_EQUAL(3, dst.analog_switch_groups_size());

  TEST_ASSERT_EQUAL(0, dst.analog_switch_groups()[0].analog_switch_group_id());
  TEST_ASSERT_EQUAL_FLOAT(
      3.5, dst.analog_switch_groups()[0].total_travel_distance());
  TEST_ASSERT_EQUAL_FLOAT(
      1.0, dst.analog_switch_groups()[0].rapid_trigger().act());

  TEST_ASSERT_EQUAL(1, dst.analog_switch_groups()[1].analog_switch_group_id());
  TEST_ASSERT_EQUAL_FLOAT(
      4.1, dst.analog_switch_groups()[1].total_travel_distance());
  TEST_ASSERT_EQUAL_FLOAT(
      0.6, dst.analog_switch_groups()[1].rapid_trigger().act());

  TEST_ASSERT_EQUAL(2, dst.analog_switch_groups()[2].analog_switch_group_id());
  TEST_ASSERT_EQUAL_FLOAT(
      4.5, dst.analog_switch_groups()[2].total_travel_distance());
  TEST_ASSERT_EQUAL_FLOAT(
      0.3, dst.analog_switch_groups()[2].rapid_trigger().act());
}

void testComplementField_default_value(void) {
  RapidTriggerConfig default_config;
  RapidTriggerConfig dst;
  default_config.set_act(1);
  default_config.set_rel(2);

  dst.set_act(4.0);

  ComplementMessage(default_config, dst);

  TEST_ASSERT_EQUAL_FLOAT(4.0, dst.act());
  TEST_ASSERT_EQUAL_FLOAT(2.0, dst.rel());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(testComplementField_default_value);
  RUN_TEST(testComplementField_repeated_field);
  return UNITY_END();
}