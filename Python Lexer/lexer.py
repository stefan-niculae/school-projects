from DFA import DFA, State, Transition


class Token:
    cached_values = []

    # TODO make the type an enum and cache the values for better memory management
    def __init__(self, type, value, is_recovered=False, is_error=False, line=None, col=None):
        self.type = type
        self.value = value

        self.is_recovered = is_recovered
        self.is_error = is_error

        self.line = line
        self.col = col


    # Use the cached list of token types (assigned by lexer)
    @property
    def type(self):
        return Token.types[self.__type_idx]

    @type.setter
    def type(self, type):
        self.__type_idx = Token.types.index(type)


    # Build and use a cached list of token values
    @property
    def value(self):
        # Return the value in the cache at the stored index
        return Token.cached_values[self.__value_idx]

    @value.setter
    def value(self, value):
        try:
            self.__value_idx = Token.cached_values.index(value)
        except ValueError:
            # Item not yet cached
            Token.cached_values.append(value)
            self.__value_idx = len(Token.cached_values) - 1



    def __str__(self):
        return '{}{} - {} @{}:{}'.format(
            self.type,
            '[R]' if self.is_recovered else '',
            self.value,
            self.line,
            self.col)

    def __eq__(self, other):
        # All it matters if it's an error token
        if self.is_error and other.is_error:
            return self.line == other.line and self.col == other.col

        return self.type == other.type and\
            self.value == other.value and\
            self.is_recovered == other.is_recovered and\
            self.is_error == other.is_error and\
            self.line == other.line and\
            self.col == other.col

# String utils
def lineno(str, index):
    # Count the newlines and add one because the first line is considered number 1
    return str[:index].count('\n') + 1

def start_of_line(str, index):
    return str[:index].rfind('\n') + 1
def end_of_line(str, index):
    return index + str[index:].find('\n')
def full_line(str, index):
    s = start_of_line(str, index)
    e = end_of_line(str, index)
    return str[s:e]

def colno(str, index):
    # Ignore previous lines (substract the number of chars up until the last line)
    return index - start_of_line(str, index) + 1


class Lexer:
    def __init__(self, token_types, dfa, ignored_chars):
        self.dfa = dfa
        self.ignored_chars = ignored_chars

        Token.types = token_types


    def parse(self, inp_str):
        i = 0
        while i < len(inp_str):
            # Skip ignored characters (whitespace)
            if inp_str[i] in self.ignored_chars:
                i += 1
                continue
            line = lineno(inp_str, i)
            col = colno(inp_str, i)

            # Search for a token from the current index onwards
            token = self.get_token(inp_str[i:])
            token.line, token.col = line, col

            if token.is_error:
                # Send the first 5 chars for preview
                remaining = inp_str[i:]
                if len(remaining) <= 5 + 3:
                    token.value = remaining
                else:
                    token.value = remaining[:5] + '...'
            else:
                # Consume the just-parsed token
                i += len(token.value)

            yield token

            # Stop after encountering an error
            if token.is_error:
                break

    def get_token(self, str):
        stop_state, matched_length, recovered  = self.dfa.run_on(str)

        matched_str = str[:matched_length]

        if not stop_state.is_final:
            return Token('*Error*', matched_str, is_error=True)

        # 'is' expects a 'not' (because there is whitespace between 'if' and 'not')
        # and thus it unreliably reports that it is recovered when matching
        # anything after the 'is' (except the expected 'not')
        # same for 'not in'
        if matched_str.startswith('is') or matched_str.startswith('not'):
            recovered = False

        return Token(stop_state.name, matched_str, recovered)

