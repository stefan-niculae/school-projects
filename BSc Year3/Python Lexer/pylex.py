#! /usr/local/bin/python3

import sys
from collections import OrderedDict

from DFA import DFA, State, Transition
from lexer import Lexer, Token


token_types = [
    # Keywords
    'Decisional Keyword',
    'Loop Control Keyword',
    'Looping Keyword',
    'Flow Control Keyword',
    'Exceptions Keyword',
    'Defining Keyword',
    'Importing Keyword',
    'Aliasing Keyword',
    'Var Ctrl Keyword',

    'Identifier',

    # Delimiters
    'Grouping Delimiter',
    'Punctuation Delimiter',
    'Period Delimiter',
    'Line Cont Delimiter',

    # Operators
    'Arith Sign Operator',
    'Arithmetic Operator',
    'Arith Assign Operator',
    'Unary Bit-Wise Operator',
    'Binary Bit-Wise Operator',
    'Bit-Wise Assign Operator',
    'Relational Operator',
    'Rel Containment Operator',
    'Assignment Operator',
    'Logical Operator',
    'Logical Neg Operator',
    'Identity Operator',
    'Neg Identity Operator',

    # Static Literals
    'Bool Literal',
    'None Literal',

    # Number Literals
    'Single Zero Literal',
    'Zero Literal',
    'Dec Int Literal',
    'Binary Literal',
    'Octal Literal',
    'Hex Literal',
    'Float Literal',
    'Complex Literal',
    'Exponent Literal',

    # String Literals
    'Short String Literal',
    'Long String Literal',
    'Short Bytes Literal',
    'Long Bytes Literal',

    # Commets
    'Comment',

    # Error
    '*Error*'
]

# It's an odered dict because -> has to be searched before -
static_tokens = OrderedDict([
    ('Keyword', OrderedDict([
        ('Decisional',      ['if', 'elif', 'else']),
        ('Looping',         ['for', 'while']),
        ('Loop Control',    ['break', 'continue']),
        ('Flow Control',    ['return', 'yield', 'assert', 'pass']), # assert before as
        ('Exceptions',      ['raise', 'try', 'except', 'finally']),
        ('Defining',        ['def', 'lambda', 'class']),
        ('Importing',       ['import', 'from']),
        ('Aliasing',        ['with', 'as']),
        ('Var Ctrl',        ['global', 'nonlocal', 'del'])
    ])),

    ('Delimiter', OrderedDict([
        ('Grouping',        '()[]{}'),
        ('Punctuation',     ['...', ',', ':', ';', '->']),
        ('Period',          '.'),
        ('Line Cont',       '\\')
    ])),

    # It's an ordered dict because == should be detected before =
    ('Operator', OrderedDict([
        # It's an ordered list because ** should take precedence over *
        ('Arith Sign',       ['+', '-']),
        ('Arithmetic',       ['**', '*', '//', '/', '%', '@']),
        # also arithmetic assignment operators
        ('Unary Bit-Wise',   ['~']),
        ('Binary Bit-Wise',  ['&', '|', '^', '<<', '>>']),
        # also bit-wise assignment operators
        ('Relational',       ['==', '!=', '<=', '>=', '<', '>']),
        ('Rel Containment',  ['in']), # also not in
        ('Assignment',       ['=']),
        ('Logical',          ['and', 'or']),
        ('Logical Neg',      ['not']),
        ('Identity',         ['is'])   # also is not
    ])),

    ('Literal', {
        'Bool': ['True', 'False'],
        'None': ['None']
    })

    # The following are identifiers (remappable), not keywords
    # types: int, float, bool, str, NoneType
    # built-ins: sum, map, filter, join, format, type etc
    # other: self, __init__ etc
    # print is a keyword only in python 2, and this analyzes python 3
])

word_static_tokens = {
    'Decisional Keyword',
    'Loop Control Keyword',
    'Looping Keyword',
    'Flow Control Keyword',
    'Exceptions Keyword',
    'Defining Keyword',
    'Importing Keyword',
    'Aliasing Keyword',
    'Var Ctrl Keyword',

    'Relational Operator',
    'Rel Containment Operator',

    'Logical Operator',
    'Logical Neg Operator',
    'Identity Operator',
    'Neg Identity Operator',

    'Bool Literal',
    'None Literal',
}

other_base_literals = {
              # modifier, digits
    'Binary': ('bB', '01'),
    'Octal':  ('oO', '01234567'),
    'Hex':    ('xX', '0123456789abcdefABCDEF')
}

# Order matters now ('rb' before 'r')
textuals = [
    ('Bytes',  ['b', 'B', 'br', 'Br', 'bR', 'BR', 'rb', 'rB', 'Rb', 'RB']),
    ('String', ['', 'r', 'R', 'u', 'U'])
]

string_preffs = ['', 'r', 'R', 'u', 'U']

is_digit = lambda s: s.isdigit()
is_lower = lambda s: s.isalpha() and s.islower()
is_upper = lambda s: s.isalpha() and s.isupper()
is_id_start = lambda s: is_lower(s) or is_upper(s) or s == '_'
is_id_body  = lambda s: is_id_start(s) or is_digit(s)
is_nz_digit = lambda s: s.isdigit() and s != '0'
is_ordinary_char = lambda s: s.isalnum() or s in '_()[]{}+-*/%@=&|~^<>?!#~\\,.;:`'
is_text_char = lambda s: is_ordinary_char(s) or s in ' \t'
is_comm_char = lambda s: is_text_char(s) or s in '\'"'
is_legal_char = lambda s: is_comm_char(s) or s == '\n'

ignored_chars = ' \t\n'


def build_dfa():
    states = {}
    states['start'] = State()


    ### Static tokens (keywords, delimiters, operators, bool & none literals) ###
    # Go through the static tokens list
    for type, subtypes in static_tokens.items():
        for subtype, elems in subtypes.items():
            state_name = subtype + ' ' + type

            # Make a transition from the start to that state
            states['start'].transitions.append(Transition(
                condition=elems, to_state_name=state_name))
            # And make the actual state, which is final
            # TODO make it not necessary to create these empty final states
            states[state_name] = State(transitions=[], is_final=True)

    # Make the transitions from arithmetic/bit-wise to assignment +=, &= etc
    states['Arith Sign Operator'].transitions = [Transition('=', 'Arith Assign Operator')]
    states['Arithmetic Operator'].transitions = [Transition('=', 'Arith Assign Operator')]
    states['Arith Assign Operator'] = State(transitions=[], is_final=True)

    states['Binary Bit-Wise Operator'].transitions = [Transition('=', 'Bit-Wise Assign Operator')]
    states['Bit-Wise Assign Operator'] = State(transitions=[], is_final=True)

    # For accepting is not
    states['Identity Operator'].transitions = [Transition(' ', 'read is, waiting not')]
    states['read is, waiting not'] = State(transitions=[
        Transition(' \t', 'read is, waiting not'),
        Transition(['not'], 'Neg Identity Operator')])
    states['Neg Identity Operator'] = State(transitions=[], is_final=True)

    # For accepting not in
    states['Logical Neg Operator'].transitions = [Transition(' ', 'read not, waiting in')]
    states['read not, waiting in'] = State(transitions=[
        Transition(' \t', 'read not, waiting in'),
        Transition(['in'], 'Rel Containment Operator')])


    ### Textual Literals ###
    # String literals have to be placed before identifiers in order to
    # catch r'...' before thinking 'r' is an identifier

    for type, preffs in textuals:
        state_name = type + ' Literal'
        long_state_name = 'Long ' + state_name
        short_state_name = 'Short ' + state_name

        # Long strings (multiple lines)
        for pref in preffs:
            states['start'].transitions.append(Transition([pref + "'''"], 'waiting 3 single quotes for ' + type))
            states['start'].transitions.append(Transition([pref + '"""'], 'waiting 3 double quotes for ' + type))

        states['waiting 3 single quotes for ' + type] = State(transitions=[
            # Can be anything but the ending triple quote
            # so we match the triple quotes first if available
            Transition(["'''"], long_state_name),
            Transition(is_legal_char, 'waiting 3 single quotes for ' + type),
            ])
        states['waiting 3 double quotes for ' + type] = State(transitions=[
            Transition(['"""'], long_state_name),
            Transition(is_legal_char, 'waiting 3 double quotes for ' + type)
            ])

        states[long_state_name] = State(transitions=[], is_final=True)


        # Short strings (single line)
        for pref in preffs:
            states['start'].transitions.append(Transition([pref + '\''], 'waiting 1 single quote for ' + type))
            states['start'].transitions.append(Transition([pref + '"'], 'waiting 1 double quote for ' + type))

        states['waiting 1 single quote for ' + type] = State(transitions=[
            Transition(lambda s: is_text_char(s) or s == '"', 'waiting 1 single quote for ' + type),
            Transition('\'', short_state_name)])
        states['waiting 1 double quote for ' + type] = State(transitions=[
            Transition(lambda s: is_text_char(s) or s == '\'', 'waiting 1 double quote for ' + type),
            Transition('"', short_state_name)])

        states[short_state_name] = State(transitions=[], is_final=True)



    ### Identifiers ###
    # From start you can go into identifier
    # This one is the last one because it matches anything un matched before, that is legal
    states['start'].transitions.append(Transition(is_id_start, 'Identifier'))
    # The identifier goes back into itself
    states['Identifier'] = State(transitions=Transition(is_id_body, 'Identifier'), is_final=True)

    # Keywords, bool and none literals are valid identifier start points
    for state_name in states:
        if state_name in word_static_tokens:
            states[state_name].transitions.append(Transition(is_id_body, 'Identifier'))



    ### Number literals ###
    # Zeros
    states['start'].transitions.append(Transition('0', 'Single Zero Literal'))
    # The zero can be signed (+ or -)
    states['Arith Sign Operator'].transitions.append(Transition('0', 'Single Zero Literal'))
    states['Single Zero Literal'] = State(transitions=[
        Transition('0', 'Zero Literal'),
        # Python3 does not allow leading zeros (eg: 01, 007)
        # but it does allow complex literals with leading zeros (eg: 01j, 007J)
        Transition(is_nz_digit, 'read leading zeros int, waiting j')
        ], is_final=True)

    states['read leading zeros int, waiting j'] = State(transitions=Transition(is_digit, 'read leading zeros int, waiting j'))

    states['Zero Literal'] = State(transitions=Transition('0', 'Zero Literal'), is_final=True)


    # Plain ints
    states['start'].transitions.append(Transition(is_nz_digit, 'Dec Int Literal'))
    # Ints can be signed
    states['Arith Sign Operator'].transitions.append(Transition(is_digit, 'Dec Int Literal'))
    states['Dec Int Literal'] = State(transitions=Transition(is_digit, 'Dec Int Literal'), is_final=True)


    # Non decimal base integer literals (bin/oct/hex floats do not exist in python)
    for base, (modifier, digits) in other_base_literals.items():
        interm_state_name = 'waiting ' + base + ' digit'
        final_state_name = base + ' Literal'

        states['Single Zero Literal'].transitions.append(Transition(modifier, interm_state_name))
        states[interm_state_name] = State(transitions=Transition(digits, final_state_name))
        states[final_state_name] = State(transitions=Transition(digits, final_state_name), is_final=True)


    # Floats
    states['Zero Literal'].transitions.append(Transition(is_nz_digit, 'read digits, waiting period'))
    states['read digits, waiting period'] = State(transitions=Transition(is_digit, 'read digits, waiting period'))

    # Any of the states below can be continued with a period into a float literal
    for from_state in ['Single Zero Literal', 'Zero Literal', 'read digits, waiting period', 'Dec Int Literal']:
        states[from_state].transitions.append(Transition('.', 'Float Literal'))

    states['Period Delimiter'].transitions = [Transition(is_digit, 'Float Literal')]
    states['Float Literal'] = State(transitions=Transition(is_digit, 'Float Literal'), is_final=True)


    # Exponentials
    # Any of the states below can be continued with an exponential modifier
    for from_state in ['Float Literal', 'Single Zero Literal', 'Zero Literal',
                       'Dec Int Literal', 'read digits, waiting period',
                       'Hex Literal']:
        states[from_state].transitions.append(Transition('eE', 'read exp modifier, waiting sign or digit'))


    states['read exp modifier, waiting sign or digit'] = State(transitions=[
        Transition('+-', 'read exp sign, waiting digit'),
        Transition(is_digit, 'Exponent Literal')])

    states['read exp sign, waiting digit'] = State(transitions=Transition(is_digit, 'Exponent Literal'))
    states['Exponent Literal'] = State(transitions=Transition(is_digit, 'Exponent Literal'), is_final=True)


    # Complex
    for from_state in ['Float Literal', 'Single Zero Literal', 'Zero Literal',
                        'Dec Int Literal', 'read digits, waiting period', 'Exponent Literal',
                        'read leading zeros int, waiting j']:
        states[from_state].transitions.append(Transition('jJ', 'Complex Literal'))

    states['Complex Literal'] = State(transitions=[], is_final=True)



    ### Comments ###
    states['start'].transitions.append(Transition('#', 'Comment'))
    states['Comment'] = State(transitions=Transition(is_comm_char, 'Comment'), is_final=True)
    # Python does not have multi-line comments
    # instead, multi-line strings that are not assigned to anything are treated like those


    # Python does not allow identifiers to start with numbers
    # We assign this now because we should be able to go from an int literal
    # with 'e' to exponent literal and with 'j' to complex literal
    for from_state in ['Dec Int Literal', 'Binary Literal', 'Octal Literal', 'Hex Literal']:
        states[from_state].transitions.append(Transition(is_id_body, 'Force Invalid'))

    states['Force Invalid'] = State(transitions=[], is_final=False)

    return DFA(states)



def build_python_lexer():
    dfa = build_dfa()
    return Lexer(token_types, dfa, ignored_chars)

def testable_format(tokens):
    def replace_escapables(s):
        return s\
            .replace('\\', '\\\\')\
            .replace('\'', "\\'")\
            .replace('\n', '\\n')\
            .replace('\t', '\\t')

    return ',\n'.join(
        "Token('{}', '{}', {}, {}, {}, {})".format(
            t.type, replace_escapables(t.value),
            t.is_recovered, t.is_error,
            t.line, t.col)
        for t in tokens)

def value_display_format(val):

    def truncate_big(s):
        max_shown = 16
        side_len = max_shown // 2
        # 3 for the elipsis ...
        if len(s) <= side_len + 3 + side_len:
            return s
        return s[:side_len] + '...' + s[-side_len:]

    def replace_newline_tab(s):
        return s\
            .replace('\n', '\\n')\
            .replace('\t', '\\t')

    t = truncate_big(val)
    return replace_newline_tab(t)


def main(argv):
    with open(argv[1]) as f:
        inp_str = f.read()

    lexer = build_python_lexer()
    tokens = lexer.parse(inp_str)

    if len(argv) > 2 and argv[2] in ['-t', '--testable']:
        print(testable_format(tokens))
        return

    # Print the headers
    print('{}  {} {:>25} {} {}'.format('Line', 'Col', 'Type', 'Rcvd', 'Value'))
    # List the tokens
    for token in tokens:
        print('{:4} {:4} {:>25}  {}   {}'.format(
            token.line,
            token.col,
            token.type,
            'r' if token.is_recovered else ' ',
            value_display_format(token.value)))


# Execute only if run as a script, not imported
if __name__ == "__main__":
    main(sys.argv)

