#include "glsltok.h"

int main()
{
    std::vector<GLSLTok::Token> tokens =
        GLSLTok::Tokenize(
            R"(
            0.3, .4lf, 3.14;
            0xfff, 0xffffu, -1u, 30000, 0XA01, 023;
            #vertex
            #define SOME_STUPID_MACRO 56.0
                in vec3 Position;
                in mat4 MatrixModel;
                in mat4 MatrixView;
                in mat4 MatrixProjection;
                out vec3 PositionWorld;
                PositionWorld = MatrixProjection * MatrixView * MatrixModel * Position;
            #vertex
                in vec3 Normal;
                in mat4 MatrixModel;
                out vec3 NormalModel = (MatrixModel * vec4(Normal, 0.0)).xyz;
            #fragment
                // TODO: Some things
                /* Multi
                    line
                    comment */
                out vec4 fragColor;
                int main()
                {
                    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
                }
            )"
        );
    
    GLSLTok::Print(tokens);
    std::getchar();
    
    return 0;
}
