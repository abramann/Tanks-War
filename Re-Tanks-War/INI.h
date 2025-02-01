#pragma once

#include "File.hpp"
#include "Types.h"

class CINI {
public:
    CINI() = delete;
    CINI(const std::string& filename);

    void setSection(const std::string& section);

   void readString(const std::string& key, std::string& value) const;
   void readInteger(const std::string& key, int& value) const;
   void readFloat(const std::string& key, float& value) const;

   void writeString(const std::string& key, const std::string& value);
   void writeInteger(const std::string& key, const int& value);
   void writeFloat(const std::string& key, const float& value);
   
   void readArrayString(const std::string& key, const Ref<std::string>& pArray) const;
   void readArrayInteger(const std::string& key, const Ref<int>& pArray) const;
   void readArrayFloat(const std::string& key, const Ref<float>& pArray) const;
   void readArray2DInteger(const std::string& key, const Ref<int*>& pArray) const;

   void writeArrayString(const std::string& key, const Ref<std::string>& pArray);
   void writeArrayInteger(const std::string& key, const Ref<int>& pArray);
   void writeArrayFloat(const std::string& key, const Ref<float>& pArray);

private:
	std::string m_section;
	mutable evt::PlainTextFileIO m_hFile;
};