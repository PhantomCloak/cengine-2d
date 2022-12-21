#pragma once

enum SerializableType {
  SR_EDITOR,    // Editor only save
  SR_READWRITE, // User save
  SR_READ       // Static
};

// Indicates that entity can be serializable
struct Serializable{
  SerializableType Type;
};
