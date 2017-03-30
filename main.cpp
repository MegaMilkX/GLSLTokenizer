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
        FLOAT_LITERAL,
        INT_LITERAL,
        DONTCARE,
        UNKNOWN
    };
    Token()
    : type(UNKNOWN) {}
    
    void Print()
    {
        
        std::cout << data;
    }
    
    std::string data;
    TYPE type;
};

static bool EatMatching(std::string& source, const std::string& token)
{
    if(BeginsWith(source, token))
    {
        source.erase(source.begin(), source.begin() + token.size() - 1);
        return true;
    }
    return false;
}

static Token TokenOperator(std::string& source)
{
    Token token;
    std::vector<std::string> ops =
        {
            "(", ")", "[", "]", ".",
            "++", "+=", "+",
            "--", "-=", "-",
            "~", "!=", "!",
            "*=", "*", "/=", "/", "%=", "%",
            "<<=", "<<", "<=", "<", 
            ">>=", ">>", ">=", ">",
            "==", "=", 
            "&&", "&=", "&",
            "^^", "^=", "^",
            "||", "|=", "|",
            "?", ":",
            ","
        };
    for(unsigned i = 0; i < ops.size(); ++i)
    {
        if(EatMatching(source, ops[i]))
        {
            token.data = ops[i];
            token.type = Token::OPERATOR;
            source.erase(source.begin(), source.begin() + ops[i].size());
            break;
        }
    }
    return token;
}

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

bool TokFloatingSuffix(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(BeginsWith(source.substr(offset), "lf") ||
        BeginsWith(source.substr(offset), "LF"))
    {
        offset+=2;
        return true;
    }
    else if(source[offset] == 'f' ||
        source[offset] == 'F')
    {
        offset++;
        return true;
    }
    
    return false;
}

bool TokDigitSequence(std::string& source, int& offset)
{
    int tmp_offset = offset;
    while(offset < source.size() &&
            IsAnyOf(source[offset], {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' })
        )
    {
        offset++;
    }
    
    return tmp_offset != offset;
}

bool TokSign(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(source[offset] == '+' ||
        source[offset] == '-')
    {
        offset++;
        return true;
    }
    
    return false;
}

bool TokExponentPart(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    int tmp_offset = offset;
    
    if(source[offset] == 'e' ||
        source[offset] == 'E')
    {
        offset++;
        TokSign(source, offset);
        
        if(TokDigitSequence(source, offset))
        {
            return true;
        }
    }
    offset = tmp_offset;
    return false;
}

bool TokFractionalConstant(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    int tmp_offset = offset;
    if(source[offset] == '.')
    {
        offset++;
        if(TokDigitSequence(source, offset))
        {
            return true;
        }
    }
    else if(TokDigitSequence(source, offset))
    {
        if(offset >= source.size())
        {
            offset = tmp_offset;
            return false;
        }
        if(source[offset] == '.')
        {
            offset++;
            TokDigitSequence(source, offset);
            return true;
        }
    }
    offset = tmp_offset;
    return false;
}

bool TokFloatingConstant(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    int tmp_offset = offset;
    if(TokFractionalConstant(source, offset))
    {
        TokExponentPart(source, offset);
        TokFloatingSuffix(source, offset);
        return true;
    }
    else if(TokDigitSequence(source, offset))
    {
        if(TokExponentPart(source, offset))
        {
            TokFloatingSuffix(source, offset);
            return true;
        }
            
    }
    offset = tmp_offset;
    return false;
}

static Token TokenFloatLiteral(std::string& source)
{
    Token token;
    int offset = 0;
    if(TokFloatingConstant(source, offset))
    {
        token.data = source.substr(0, offset);
        token.type = Token::FLOAT_LITERAL;
        source.erase(source.begin(), source.begin() + offset);
    }
    
    return token;
}

bool TokHexDigit(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(IsAnyOf(source[offset], { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                 'a', 'b', 'c', 'd', 'e', 'f',
                                 'A', 'B', 'C', 'D', 'E', 'F'
                               }
              )
      )
    {
        offset++;
        return true;
    }
    
    return false;
}

bool TokOctDigit(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(IsAnyOf(source[offset], { '0', '1', '2', '3', '4', '5', '6', '7' }))
    {
        offset++;
        return true;
    }
    
    return false;
}

bool TokNonzeroDigit(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(IsAnyOf(source[offset], { '1', '2', '3', '4', '5', '6', '7', '8', '9' }))
    {
        offset++;
        return true;
    }
    
    return false;
}

bool TokDigit(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(source[offset] == '0')
    {
        offset++;
        return true;
    }
    else
        return TokNonzeroDigit(source, offset);
}

bool TokHexDigits(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(IsAnyOf(source[offset], { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                 'a', 'b', 'c', 'd', 'e', 'f',
                                 'A', 'B', 'C', 'D', 'E', 'F' }))
    {
        offset++;
        return TokHexDigits(source, offset);
    }
    
    return false;
}

bool TokOctDigits(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(IsAnyOf(source[offset], { '0', '1', '2', '3', '4', '5', '6', '7' }))
    {
        offset++;
        return TokOctDigits(source, offset);
    }
    
    return false;
}

bool TokDecDigits(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(IsAnyOf(source[offset], { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }))
    {
        offset++;
        return TokDecDigits(source, offset);
    }
    
    return false;
}

bool TokHexConstant(std::string& source, int& offset)
{
    if(offset+1 >= source.size())
        return false;
    
    if(BeginsWith(source.substr(offset), "0x") ||
        BeginsWith(source.substr(offset), "0X"))
    {
        offset+=2;
        if(TokHexDigits(source, offset))
            return true;
    }
    
    return false;
}

bool TokOctConstant(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(source[offset] == '0')
    {
        offset++;
        TokOctDigits(source, offset);
        return true;
    }
    
    return false;
}

bool TokDecConstant(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(TokNonzeroDigit(source, offset))
    {
        TokDecDigits(source, offset);
        return true;
    }
    
    return false;
}

bool TokIntegerSuffix(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(source[offset] == 'U' ||
        source[offset] == 'u')
    {
        offset++;
        return true;
    }
    
    return false;
}

bool TokIntegerConstant(std::string& source, int& offset)
{
    if(offset >= source.size())
        return false;
    
    if(TokDecConstant(source, offset))
    {
        TokIntegerSuffix(source, offset);
        return true;
    }
    else if(TokHexConstant(source, offset))
    {
        TokIntegerSuffix(source, offset);
        return true;
    }
    else if(TokOctConstant(source, offset))
    {
        TokIntegerSuffix(source, offset);
        return true;
    }
    
    return false;
}

static Token TokenIntLiteral(std::string& source)
{
    Token token;
    
    int offset = 0;
    if(TokIntegerConstant(source, offset))
    {
        token.data = source.substr(0, offset);
        token.type = Token::INT_LITERAL;
        source.erase(source.begin(), source.begin() + offset);
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
            else if((token = TokenFloatLiteral(src)).type != Token::UNKNOWN)
            {
                tokens.push_back(token);
                continue;
            }
            else if((token = TokenIntLiteral(src)).type != Token::UNKNOWN)
            {
                tokens.push_back(token);
                continue;
            }
            else if((token = TokenOperator(src)).type != Token::UNKNOWN)
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
            //token.data.push_back(src.front());
            //tokens.push_back(token);
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
            out vec3 lambertOmni16
        )"
    );
    
    tokenizer.Print();
    std::getchar();
    
    return 0;
}
