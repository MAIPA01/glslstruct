# glslstruct

## About

**glslstruct** is a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.

It allows you to define structures once and retrieve their precise size and field offsets according to GLSL packing rules, such as **`std140`** or **`std430`**, eliminating the need for manual padding and tedious calculations.

---

## 🚀 Key Features

* **GLSL Standard Compliance:** Built-in support for the most common GLSL packing rules (`std140`, `std430`).
* **User-Friendly Interface:** Easy-to-use interface for defining and using structures.

---

## 🛠️ Requirements

* C++17 compliant compiler (GCC 10+, Clang 10+, MSVC 19.29+).
* CMake (3.24+) (optional, for building examples and testing).

---

## Installation

### Method 1: Using CMake (Recommended)

1.  Clone the repository:
    ```bash
    git clone [https://github.com/MAIPA01/glslstruct.git](https://github.com/MAIPA01/glslstruct.git)
    ```

2.  In your project's `CMakeLists.txt`, add the library and link it:
    ```cmake
    add_subdirectory(glslstruct)
    
    target_link_libraries(YourApplicationName PRIVATE glslstruct)
    ```

---

## 💡 Example Usage

The following code demonstrates defining a structure and querying its metadata according to the `std140` layout, which is essential for binding UBOs correctly.

```cpp
#include <glslstruct/glslstruct.hpp>
#include <iostream>
#include <vector>
#include <glm/glm.hpp> // Is used for vectors and matrixes

using namespace glslstruct;

// 1. Define your C++ structure
std140_struct SceneSettings {
    std_value<glm::mat4>("viewProjection"),
    std_value<glm::vec3>("cameraPosition"),
    std_value<float>("globalTime"),
    std_value<int>("renderMode")
};

int main() {    
    // Total aligned size of the structure
    std::cout << "Total UBO size (std140): " << SceneSettings.size() << " bytes." << std::endl; // Should be a multiple of 16 bytes
    
    // Offset for a specific field using a literal string
    std::cout << "Offset for 'cameraPosition': " << SceneSettings.getOffset("cameraPosition") << " bytes." << std::endl;
    
    // Get data for uploading to UBO
    std::vector<std::byte> data_to_upload = SceneSettings.data();

    // Set variable
    SceneSettings.set("cameraPosition", glm::vec3{ 1.f, 0.f, 5.f });

    // Get variable
    glm::vec3 cameraPosition = SceneSettings.get("cameraPosition");

    // Get type
    const base_type* value_type = SceneSettings.getType("cameraPosition");

    return 0;
}
```

# 📝 GLSL Shader Example
This is how the corresponding GLSL UBO definition would look for the SceneSettings structure above:

```glsl
// GLSL Side:
layout(std140, binding = 0) uniform SceneSettingsUBO
{
    mat4 viewProjection;
    vec3 cameraPosition;
    float globalTime;
    int renderMode;
};
```

# 📝 Operations On Types
In structures there are five diffrent type handlers:
- `scalar_type`
- `vec_type`
- `mat_type`
- `struct_type`
- `array_type`

all of them with the same base class `base_type`. 

### Type Casting
For easier and safer casting instead of using `dynamic_cast` there is implemented function `dynamic_type_cast` and `static_type_cast`.

### Visitor
Each type has implemented `accept` function which as a input accepts pointer to `base_type_visitor` class.

#### Example
```cpp
#include <glslstruct/glslstruct.hpp>
#include <iostream>
#include <string>
#include <glm/glm.hpp>

using namespace glslstruct;

std140_struct SceneSettings {
    std_value<glm::mat4>("viewProjection"),
    std_value<glm::vec3>("cameraPosition"),
    std_value<float>("globalTime"),
    std_value<int>("renderMode")
};

// Visitor Definition
class zero_values_visitor : public base_type_visitor {
private:
    std::string _valueName;

public:
    zero_values_visitor() = default;
    virtual ~zero_values_visitor() = default;

    void setValueName(const std::string& name) {
        _valueName = name;
    }

    void visit(const scalar_type& type) {
        switch(type.type()) {
        case ValueType::Int:
            SceneSettings.set(_valueName, 0);
            break;
        case ValueType::Float:
            SceneSettings.set(_valueName, 0.f);
            break;
        }
    }
    void visit(const vec_type& type) {
        if (type.type() == ValueType::Float && type.length() == 3) {
            SceneSettings.set(_valueName, glm::vec3{ 0.f, 0.f, 0.f });
        }
    }
    void visit(const mat_type& type) {
        if (type.type() == ValueType::Float && type.cols() == 4 && type.rows() == 4) {
            SceneSettings.set(_valueName, glm::mat4(0.f));
        }
    }
    void visit(const struct_type& type) {

    }
    void visit(const array_type& type) {

    }
}

int main() {

    zero_values_visitor visitor{};
    for (const auto& name : SceneSettings.getNames()) {
        visitor.setValueName(name);
        SceneSettings.getType(name)->accept(&visitor);
    }

    return 0;
}
```

# 📜 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

# 🤝 Contributing
We welcome all contributions! Whether it's reporting a bug, suggesting a new feature, or submitting a pull request, your input is valued.