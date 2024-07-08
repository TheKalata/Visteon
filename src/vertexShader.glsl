R"(
        #version 300 es

        layout(location = 0) in vec3 position;
        // layout(location = 3) in vec3 color;
        // out vec3 vColor;
        void main()
        {
            // vColor = color;
            gl_Position = vec4(position, 1.0);
        }
    )"