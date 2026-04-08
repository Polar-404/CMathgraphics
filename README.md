# CMathGraphics

A low-level math function plotter written in C (computation) and C++ with OpenGL (rendering). Type any expression and see it plotted in real time.

---

## How it works

### Mathematical Model
The engine evaluates the input as a function of $x$:
$$y = f(x)$$
Where $x$ is sampled across the domain (e.g., $\in[-10, 10]$) to produce the vertices of the graph.

### Computation pipeline (C)

The expression you type goes through four stages before any pixel is drawn:


<p align="center"> 
  <code>String</code> → <code>Tokens (Infix)</code> → <code>Tokens (RPN)</code> → <code>Node Tree</code> → <code>eval_node(x)</code> 
</p>

**1. Tokenization** — the raw string is scanned character by character and broken into typed tokens: numbers, variables, operators, functions, and parentheses.

**2. Infix → RPN ([Shunting-Yard](https://en.wikipedia.org/wiki/Shunting_yard_algorithm))** — the token list is rearranged into Reverse Polish Notation, which eliminates the need for parentheses and encodes operator precedence explicitly. For example:

$$ 3 + 4 \times x \implies 3 \ 4 \ x \ \times \ + $$

**3. AST construction** — the RPN token stream is consumed to build a binary expression tree (AST), where each node is either an operator, a function call, or a leaf value.

**4. Evaluation** — `eval_node(root, x)` walks the tree recursively for each value of `x` in the domain `[-10, 10]`, producing a stream of `(x, f(x))` pairs.

---

### Rendering pipeline (C++ / OpenGL)

Points are uploaded to the GPU as a VBO and drawn with `GL_LINE_STRIP`, connecting samples in order to form the curve. The coordinate range is scaled so that the full domain fits the viewport:

```cpp
points.push_back(static_cast<float>(x * 0.1));       // X: maps [-10, 10] → [-1, 1]
points.push_back(static_cast<float>(result * 0.1));   // Y
points.push_back(0.0f);                               // Z (2D)
```

The grid is drawn separately via a fullscreen quad — a fragment shader runs over every pixel and decides whether it falls on a grid line, using the window dimensions passed as uniforms.

---

### Main loop

The program runs two threads concurrently: one handles console input, the other runs the render loop. An atomic flag `new_expr` signals when a new expression is ready.

```cpp
init_opengl();
std::thread input_thread(user_interface);

while (true) {
    if (new_expr) {
        new_expr = false;
        current_expr = calc_expr_tree(math_expr); // parse + upload to GPU
    }
    render_frame(); // draw grid + curve
}
```

---

## Supported syntax

| Token type | Examples |
|---|---|
| Numbers | `3`, `1.5`, `.7` |
| Variable | `x` |
| Operators | `+`, `-`, `*`, `/`, `^` |
| Functions | `sin`, `cos`, `tan`, `log`, `sqrt`, ... |
| Grouping | `(`, `)` |

**Example expressions:**

```
sin(x)
x^2 - 3*x + 2
sin(x) / x
sqrt(1 - x^2)
```

---
## Visualizations

<div align="center">

| cos(x * 2) | sin(x) / x |
| :---: | :---: |
| <img width="350" alt="cos(x * 2)" src="https://github.com/user-attachments/assets/461f199b-a8cb-464b-9564-ee8937d8d016" /> | <img width="350" alt="sin(x) / x" src="https://github.com/user-attachments/assets/a6191959-cbf0-4c1f-8b67-ddd8ec597a5b" /> |
| **floor(x) * 0.5** | **abs(exp(0.5 * x)) * sin(10 * x)** |
| <img width="350" alt="floor(x) * 0.5" src="https://github.com/user-attachments/assets/5ee9454b-6aaa-4f12-85a1-ac984a8d7b08" /> | <img width="350" alt="abs(exp(0.5 * x)) * sin(10 * x)" src="https://github.com/user-attachments/assets/7f941483-7198-4186-ae21-61e5ca4a5cd7" /> |

</div>

---

## Building

> Dependencies: OpenGL, GLFW, GLAD

```bash
# placeholder
cmake -B build && cmake --build build
```

---

## License
