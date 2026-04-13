# glslstruct

## About

**glslstruct** is a C++ library designed to easily represent GLSL's Uniform Buffer Objects (UBOs) and Shader Storage Buffer Objects (SSBOs) in C++.

It allows you to define structures once and retrieve their precise size and field offsets according to GLSL packing rules, such as **`std140`**, **`std430`** or **`scalar`**, eliminating the need for manual padding and tedious calculations. [DOCS](https://maipa01.github.io/glslstruct/docs/html)

---

## 🚀 Key Features

* **GLSL Standard Compliance:** Built-in support for the most common GLSL packing rules (`std140`, `std430`, `scalar`).
* **User-Friendly Interface:** Easy-to-use interface for defining and using structures.
* **Builtin Parser:** Parser which helps with constructing structures representation based on glsl code
* **Builtin Writer:** Helps generate glsl code of structures for OpenGL and Vulkan

---

## Options
Options can be enabled in c++ by `#define option_name` or by enabling cmake option `set(option_name CACHE ON)`

| Cmake Option/C++ Define                 | Description                                          | Default |
|:----------------------------------------|:-----------------------------------------------------|:-------:|
| `GLSL_STRUCT_ENABLE_CXX20`              | Enables C++20 features                               |   OFF   |
| `GLSL_STRUCT_DISABLE_ASSERT_ON_RELEASE` | Disables assert on release builds                    |   OFF   |
| `GLSL_STRUCT_DISABLE_TYPES`             | Disables type classes                                |   OFF   |
| `GLSL_STRUCT_ENABLE_TYPE_CHECKS`        | Enables type checking only if types are not disabled |   OFF   |
| `GLSL_STRUCT_DISABLE_PARSER`            | Disables parsers functions                           |   OFF   |

---

## 🛠️ Requirements

* C++17 compliant compiler (GCC 10+, Clang 10+, MSVC 19.29+).
* CMake (3.30+) (optional, for building examples and testing).

---

## Installation

### Method 1: Using CMake (Recommended)

1.  Clone the repository:
    ```bash
    git clone https://github.com/MAIPA01/glslstruct.git
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
    glsl_value<glm::mat4>("viewProjection"),
    glsl_value<glm::vec3>("cameraPosition"),
    glsl_value<float>("globalTime"),
    glsl_value<int>("renderMode")
};

int main() {    
    // Total aligned size of the structure
    std::cout << "Total UBO size (std140): " << SceneSettings.size() 
        << " bytes." << std::endl; // Should be a multiple of 16 bytes
    
    // Offset for a specific field using a literal string
    std::cout << "Offset for 'cameraPosition': " << SceneSettings.get_offset("cameraPosition") 
        << " bytes." << std::endl;
    
    // Get data for uploading to UBO
    std::vector<std::byte> data_to_upload = SceneSettings.data();

    // Set variable
    SceneSettings.set("cameraPosition", glm::vec3{ 1.f, 0.f, 5.f });

    // Get variable
    glm::vec3 cameraPosition = SceneSettings.get("cameraPosition");

    // Get type
    base_type_handle value_type = SceneSettings.get_type("cameraPosition");
    vec_type_handle vec_value_type = static_type_cast<vec_type>(value_type);

    std::cout << "Vector type: " << to_string(vec_value_type->get_type()) 
        << " length: " << vec_value_type->get_length() << std::endl;

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

You can also use `std140_parser` class to get structure layout from this glsl code or use `glsl_opengl_writer` class to generate glsl code like this from structure defined in code

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
Each type has implemented `accept` function which as a input accepts all types which match with `type_visitor_concept`.

#### Example
```cpp
#include <glslstruct/glslstruct.hpp>
#include <iostream>
#include <string>
#include <string_view>
#include <glm/glm.hpp>
#include <fmt/fmt.h>

using namespace glslstruct;

std140_struct SceneSettings {
    glsl_value<glm::mat4>("viewProjection"),
    glsl_value<glm::vec3>("cameraPosition"),
    glsl_value<float>("globalTime"),
    glsl_value<int>("renderMode")
};

// Visitor Definition
class draw_visitor {
private:
    std::string _varName;

public:
    void setVarName(const std::string_view name) {
        _varName = name;
    }

    void visit(const scalar_type& type) {
        switch(type.get_type()) {
        case ValueType::Int:
            // Some simple GUI draw function
            draw_int(_varName, SceneSettings.get<int>(_varName));
            break;
        case ValueType::Float:
            // Some simple GUI draw function
            draw_float(_varName, SceneSettings.get<float>(_varName));
            break;
        }
    }
    void visit(const vec_type& type) {
        if (type.get_type() == ValueType::Float && 
            type.get_length() == 3) {
            // Some simple GUI draw function
            draw_vec3(_valueName, SceneSettings.get<glm::vec3>(_varName));
        }
    }
    void visit(const mat_type& type) {
        if (type.get_type() == ValueType::Float && 
            type.get_cols() == 4 && 
            type.get_rows() == 4) {
            // Some simple GUI draw function
            draw_mat4(_valueName, SceneSettings.get<glm::mat4>(_varName));
        }
    }
    void visit(const struct_type& type) {
        const std::string structName = _valueName;
    
        draw_begin_sub_menu(structName);
        for (const auto& [name, varData] : type->get_top_level_variables()) {
            _valueName = fmt::format("{}.{}", structName, name);
            varData.get_type()->accept(*this);
        }
        draw_end_sub_menu();
    }
    void visit(const array_type& type) {
        const std::string arrayName = _valueName;
    
        draw_begin_list(arrayName);
        for (size_t i = 0; i < type.get_count(); ++i) {
            _valueName = fmt::format("{}[{}]", arrayName, i);
            type.get_type()->accept(*this);
        }
        draw_end_list();
    }
}

int main() {

    draw_visitor visitor{};
    for (const auto& [name, varData] : SceneSettings.get_top_level_variables()) {
        visitor.setValueName(name);
        varData.get_type()->accept(visitor);
    }

    return 0;
}
```

# 📜 License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

# 🤝 Contributing
We welcome all contributions! Whether it's reporting a bug, suggesting a new feature, or submitting a pull request, your input is valued.
