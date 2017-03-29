#include <vector>
#include <string>
#include <iostream>

bool IsAnyOf(char c, const std::vector<char>& chars)
{
    for(unsigned i = 0; i < chars.size(); ++i)
    {
        if(c == chars[i])
            return true;
    }
    return false;
}

bool BeginsWith(const std::string& string, const std::string& substring)
{
    if(string.substr(0, substring.size()) == substring)
        return true;
    else 
        return false;
}

struct Token
{
    enum TYPE
    {
        IDENTIFIER,
        OPERATOR,
        WHITESPACE,
        SEMICOLON,
        PREPROCESSOR,
        UNKNOWN
    };
    Token()
    : type(UNKNOWN) {}
    
    void Print()
    {
        std::cout << data << std::endl;
    }
    
    std::string data;
    TYPE type;
};

static Token TokenIdentifier(std::string& source)
{
    std::vector<char> firstCharacters = 
        { '_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
          'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 
          'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
          'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 
          'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
          'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' 
        };
    std::vector<char> characters = 
        { '_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
          'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 
          'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
          'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 
          'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
          'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0',
          '1', '2', '3', '4', '5', '6', '7', '8', '9'
        };
    
    Token token;
    int charIndex = 0;
    while(charIndex < source.size())
    {
        if(token.data.empty())
        {
            if(!IsAnyOf(source[charIndex], firstCharacters))
                return token;
            token.data.push_back(source[charIndex]);
            charIndex++;
        }
        else
        {
            if(!IsAnyOf(source[charIndex], characters))
            {
                break;
            }
            token.data.push_back(source[charIndex]);
            charIndex++;
        }
    }
    
    source.erase(source.begin(), source.begin() + charIndex);
    token.type = Token::IDENTIFIER;
    return token;
}

static Token TokenSemicolon(std::string& source)
{
    Token token;
    if(source.front() == ';')
    {
        token.data.push_back(source.front());
        token.type = Token::SEMICOLON;
        source.erase(source.begin());
    }
    return token;
}

static Token TokenPreprocessor(std::string& source)
{
    std::vector<char> characters = 
        { '_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 
          'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 
          'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
          'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 
          'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 
          'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
        };
    Token token;
    if(source.front() == '#')
    {
        token.data.push_back(source.front());
        token.type = Token::PREPROCESSOR;
        source.erase(source.begin());
        
        int charIndex = 0;
        while(charIndex < source.size())
        {
            if(source[charIndex] == '\n')
            {
                break;
            }
            token.data.push_back(source[charIndex]);
            charIndex++;
        }
        
        source.erase(source.begin(), source.begin() + charIndex);
    }
    return token;
}

static Token TokenWhitespace(std::string& source)
{
    Token token;
    if(source.front() == 32 ||
        source.front() == 9 ||
        source.front() == 10 ||
        source.front() == 11 ||
        source.front() == 12 ||
        source.front() == 13)
    {
        token.data.push_back(source.front());
        token.type = Token::WHITESPACE;
        source.erase(source.begin());
        
        int charIndex = 0;
        while(charIndex < source.size())
        {
            if(!(source[charIndex] == 32 ||
                source[charIndex] == 9 ||
                source[charIndex] == 10 ||
                source[charIndex] == 11 ||
                source[charIndex] == 12 ||
                source[charIndex] == 13))
            {
                break;
            }
            token.data.push_back(source[charIndex]);
            charIndex++;
        }
        
        source.erase(source.begin(), source.begin() + charIndex);
    }
    return token;
}

class GLSLTokenizer
{
public:    
    void Tokenize(const std::string& source)
    {
        std::string src = source;
        while(!src.empty())
        {
            Token token;
            if((token = TokenIdentifier(src)).type != Token::UNKNOWN)
            {
                tokens.push_back(token);
                continue;
            }
            else if((token = TokenSemicolon(src)).type != Token::UNKNOWN)
            {
                tokens.push_back(token);
                continue;
            }
            else if((token = TokenPreprocessor(src)).type != Token::UNKNOWN)
            {
                tokens.push_back(token);
                continue;
            }
            else if((token = TokenWhitespace(src)).type != Token::UNKNOWN)
            {
                tokens.push_back(token);
                continue;
            }
            src.erase(src.begin());
        }
    }
    
    void Print()
    {
        for(unsigned i = 0; i < tokens.size(); ++i)
        {
            tokens[i].Print();
        }
    }
private:
    std::vector<Token> tokens;
};

int main()
{
    GLSLTokenizer tokenizer;    
    
    tokenizer.Tokenize(
        R"(
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
            out vec3 lambertOmni16
        )"
    );
    
    tokenizer.Print();
    std::getchar();
    
    return 0;
}
