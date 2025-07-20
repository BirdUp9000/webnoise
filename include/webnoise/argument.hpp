#pragma once

#include <string>

struct CArgument final {
  std::string m_Name;
  std::string m_TextLeft;
  std::string m_TextRight;

  float m_Value = 0.0f;
  float m_MinValue = 0.0f;
  float m_MaxValue = 0.0f;
};