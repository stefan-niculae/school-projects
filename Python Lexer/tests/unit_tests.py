import unittest

from python_lexer import build_python_lexer
from lexer import Token


def strip_indentation(s, spaces=12, remove_outer_lines=True):
    # We need this so we don't read from an outer file (you can compare easier)
    # And when we write a multiline string, it takes into account
    # the tabs used for indentation as well
    lines = s.split('\n')
    if remove_outer_lines:
        lines = lines[1:-1]
    stripped = [ l[spaces:] for l in lines ]
    return '\n'.join(stripped)


# TODO make more tests for whitespace ignoring?
# TODO make more tests for recovers
# TODO make more string tests for unrecoverables

class TestPythonLexer(unittest.TestCase):
    def token_list(self, inp):
        inp = strip_indentation(inp)
        lex = build_python_lexer()
        return list(lex.parse(inp))

    def test_if_else_whitespace_error(self):
        tokens = self.token_list('''
            if else if

              if ? else
            !
            ''')

        self.assertEqual(tokens, [
            # type, value, is_recovered, is_error, line, column
            Token('Decisional Keyword', 'if', False, False, 1, 1),
            Token('Decisional Keyword', 'else', False, False, 1, 4),
            Token('Decisional Keyword', 'if', False, False, 1, 9),
            Token('Decisional Keyword', 'if', False, False, 3, 3),
            Token('*Error*', '? els...', False, True, 3, 6)
        ])

    def test_delims_opers_keywords(self):
        tokens = self.token_list('''
            continue lambda   ,  del . @@
            +=; is >=
            ...
            ''')

        self.assertEqual(tokens, [
            Token('Loop Control Keyword', 'continue', False, False, 1, 1),
            Token('Defining Keyword', 'lambda', False, False, 1, 10),
            Token('Punctuation Delimiter', ',', False, False, 1, 19),
            Token('Var Ctrl Keyword', 'del', False, False, 1, 22),
            Token('Period Delimiter', '.', False, False, 1, 26),
            Token('Arithmetic Operator', '@', False, False, 1, 28),
            Token('Arithmetic Operator', '@', False, False, 1, 29),
            Token('Arith Assign Operator', '+=', False, False, 2, 1),
            Token('Punctuation Delimiter', ';', False, False, 2, 3),
            Token('Identity Operator', 'is', False, False, 2, 5),
            Token('Relational Operator', '>=', False, False, 2, 8),
            Token('Punctuation Delimiter', '...', False, False, 3, 1)
        ])

    def test_identifiers_keywords_operators_identifier_starting_with_nr_recover(self):
        tokens = self.token_list('''
            abc
            for formal
            return ret_urn
            a12+b
            del _delicious
            1a

            ''')

        self.assertEqual(tokens, [
            Token('Identifier', 'abc', False, False, 1, 1),
            Token('Looping Keyword', 'for', False, False, 2, 1),
            Token('Identifier', 'formal', False, False, 2, 5),
            Token('Flow Control Keyword', 'return', False, False, 3, 1),
            Token('Identifier', 'ret_urn', False, False, 3, 8),
            Token('Identifier', 'a12', False, False, 4, 1),
            Token('Arith Sign Operator', '+', False, False, 4, 4),
            Token('Identifier', 'b', False, False, 4, 5),
            Token('Var Ctrl Keyword', 'del', False, False, 5, 1),
            Token('Identifier', '_delicious', False, False, 5, 5),
            Token('Dec Int Literal', '1', True, False, 6, 1),
            Token('Identifier', 'a', False, False, 6, 2)
        ])

    def test_arith_and_assign_arith_operators(self):
        tokens = self.token_list('''
            if + else
                  for +=
            = ==
            ''')

        self.assertEqual(tokens, [
            Token('Decisional Keyword', 'if', False, False, 1, 1),
            Token('Arith Sign Operator', '+', False, False, 1, 4),
            Token('Decisional Keyword', 'else', False, False, 1, 6),
            Token('Looping Keyword', 'for', False, False, 2, 7),
            Token('Arith Assign Operator', '+=', False, False, 2, 11),
            Token('Assignment Operator', '=', False, False, 3, 1),
            Token('Relational Operator', '==', False, False, 3, 3)
        ])

    def test_logicals_is_isnot_in_notin(self):
        tokens = self.token_list('''
            or and . not
             is is not
            is   not
            is
            is not not

            if in
            if not in
            if not    not    in
            isinstance
            orbit
            notable
            ind
            ''')

        self.assertEqual(tokens, [
            Token('Logical Operator', 'or', False, False, 1, 1),
            Token('Logical Operator', 'and', False, False, 1, 4),
            Token('Period Delimiter', '.', False, False, 1, 8),
            Token('Logical Neg Operator', 'not', False, False, 1, 10),
            Token('Identity Operator', 'is', False, False, 2, 2),
            Token('Neg Identity Operator', 'is not', False, False, 2, 5),
            Token('Neg Identity Operator', 'is   not', False, False, 3, 1),
            Token('Identity Operator', 'is', False, False, 4, 1),
            Token('Neg Identity Operator', 'is not', False, False, 5, 1),
            Token('Logical Neg Operator', 'not', False, False, 5, 8),
            Token('Decisional Keyword', 'if', False, False, 7, 1),
            Token('Rel Containment Operator', 'in', False, False, 7, 4),
            Token('Decisional Keyword', 'if', False, False, 8, 1),
            Token('Rel Containment Operator', 'not in', False, False, 8, 4),
            Token('Decisional Keyword', 'if', False, False, 9, 1),
            Token('Logical Neg Operator', 'not', False, False, 9, 4),
            Token('Rel Containment Operator', 'not    in', False, False, 9, 11),
            Token('Identifier', 'isinstance', False, False, 10, 1),
            Token('Identifier', 'orbit', False, False, 11, 1),
            Token('Identifier', 'notable', False, False, 12, 1),
            Token('Identifier', 'ind', False, False, 13, 1)
        ])

    def test_bool_none_literals_whitespace(self):
        tokens = self.token_list('''
            True

                False

            None       True           None
            ''')

        self.assertEqual(tokens, [
            Token('Bool Literal', 'True', False, False, 1, 1),
            Token('Bool Literal', 'False', False, False, 3, 5),
            Token('None Literal', 'None', False, False, 5, 1),
            Token('Bool Literal', 'True', False, False, 5, 12),
            Token('None Literal', 'None', False, False, 5, 27),
        ])

    def test_identifier_starting_with_bool_none_literal(self):
        tokens = self.token_list('''
            ifx
            if x
            True
            TrueNone
            FalseTrue
            Truethy
            Nonea
            ''')

        self.assertEqual(tokens, [
            Token('Identifier', 'ifx', False, False, 1, 1),
            Token('Decisional Keyword', 'if', False, False, 2, 1),
            Token('Identifier', 'x', False, False, 2, 4),
            Token('Bool Literal', 'True', False, False, 3, 1),
            Token('Identifier', 'TrueNone', False, False, 4, 1),
            Token('Identifier', 'FalseTrue', False, False, 5, 1),
            Token('Identifier', 'Truethy', False, False, 6, 1),
            Token('Identifier', 'Nonea', False, False, 7, 1)
        ])

    def test_zeros_signed_unsinged_single_multiple(self):
        tokens = self.token_list('''
            0
            +0
            -0
            00
            0000000000000
            ''')

        self.assertEqual(tokens, [
            Token('Single Zero Literal', '0', False, False, 1, 1),
            Token('Single Zero Literal', '+0', False, False, 2, 1),
            Token('Single Zero Literal', '-0', False, False, 3, 1),
            Token('Zero Literal', '00', False, False, 4, 1),
            Token('Zero Literal', '0000000000000', False, False, 5, 1)
        ])

    def test_signed_unsigned_long_decimal_ints_leading_zero_recover(self):
        tokens = self.token_list('''
            2
            6
            2131232132131312
            +2
            -43143414321
            01
            007
            ''')

        self.assertEqual(tokens, [
            Token('Dec Int Literal', '2', False, False, 1, 1),
            Token('Dec Int Literal', '6', False, False, 2, 1),
            Token('Dec Int Literal', '2131232132131312', False, False, 3, 1),
            Token('Dec Int Literal', '+2', False, False, 4, 1),
            Token('Dec Int Literal', '-43143414321', False, False, 5, 1),
            Token('Single Zero Literal', '0', True, False, 6, 1),
            Token('Dec Int Literal', '1', False, False, 6, 2),
            Token('Zero Literal', '00', True, False, 7, 1),
            Token('Dec Int Literal', '7', False, False, 7, 3)
        ])

    def test_non_dec_ints_signed_unsigned_lower_upper_out_of_range_recover(self):
        tokens = self.token_list('''
            0b1
            +0b0
            -0B110010101
            0O7324112342
            0x138FaffAAFFEDEDEC213
            0b9
            0xy
            -0XAAAAAA
            ''')

        self.assertEqual(tokens, [
            Token('Binary Literal', '0b1', False, False, 1, 1),
            Token('Binary Literal', '+0b0', False, False, 2, 1),
            Token('Binary Literal', '-0B110010101', False, False, 3, 1),
            Token('Octal Literal', '0O7324112342', False, False, 4, 1),
            Token('Hex Literal', '0x138FaffAAFFEDEDEC213', False, False, 5, 1),
            Token('Single Zero Literal', '0', True, False, 6, 1),
            Token('Identifier', 'b9', False, False, 6, 2),
            Token('Single Zero Literal', '0', True, False, 7, 1),
            Token('Identifier', 'xy', False, False, 7, 2),
            Token('Hex Literal', '-0XAAAAAA', False, False, 8, 1)
        ])

    def test_float_missing_integer_missing_fractional_normal(self):
        tokens = self.token_list('''
            0.
            0000.
            00432141.
            341314.
            .0
            .5
            .000005432524234124
            4312431.43124214
            1.2
            1.0
            ''')

        self.assertEqual(tokens, [
            Token('Float Literal', '0.', False, False, 1, 1),
            Token('Float Literal', '0000.', False, False, 2, 1),
            Token('Float Literal', '00432141.', False, False, 3, 1),
            Token('Float Literal', '341314.', False, False, 4, 1),
            Token('Float Literal', '.0', False, False, 5, 1),
            Token('Float Literal', '.5', False, False, 6, 1),
            Token('Float Literal', '.000005432524234124', False, False, 7, 1),
            Token('Float Literal', '4312431.43124214', False, False, 8, 1),
            Token('Float Literal', '1.2', False, False, 9, 1),
            Token('Float Literal', '1.0', False, False, 10, 1)
        ])

    def test_exponential_signed_unsigned_zeros_floats_ints_recovers(self):
        tokens = self.token_list('''
            0e0
            0000e99
            1.5e81341
            0004321421e2
            12e2
            12E+2
            15.E-88
            12ab
            0be5
            ''')

        self.assertEqual(tokens, [
            Token('Exponent Literal', '0e0', False, False, 1, 1),
            Token('Exponent Literal', '0000e99', False, False, 2, 1),
            Token('Exponent Literal', '1.5e81341', False, False, 3, 1),
            Token('Exponent Literal', '0004321421e2', False, False, 4, 1),
            Token('Exponent Literal', '12e2', False, False, 5, 1),
            Token('Exponent Literal', '12E+2', False, False, 6, 1),
            Token('Exponent Literal', '15.E-88', False, False, 7, 1),
            Token('Dec Int Literal', '12', True, False, 8, 1),
            Token('Identifier', 'ab', False, False, 8, 3),
            Token('Single Zero Literal', '0', True, False, 9, 1),
            Token('Identifier', 'be5', False, False, 9, 2)
        ])

    def test_complex_float_int_bin_oct_hex_exp_recovers(self):
        tokens = self.token_list('''
            1.5j
            -000.27324J
            0j
            0000j
            1j
            43241432j
            01j
            00001j
            0000412341j
            1e5j
            2.7E-12J

            0b1e2
            0o5e6
            0x2e4
            00x3
            0x33213E-43213

            8exj
            12e2ab
            ''')

        self.assertEqual(tokens, [
            Token('Complex Literal', '1.5j', False, False, 1, 1),
            Token('Complex Literal', '-000.27324J', False, False, 2, 1),
            Token('Complex Literal', '0j', False, False, 3, 1),
            Token('Complex Literal', '0000j', False, False, 4, 1),
            Token('Complex Literal', '1j', False, False, 5, 1),
            Token('Complex Literal', '43241432j', False, False, 6, 1),
            Token('Complex Literal', '01j', False, False, 7, 1),
            Token('Complex Literal', '00001j', False, False, 8, 1),
            Token('Complex Literal', '0000412341j', False, False, 9, 1),
            Token('Complex Literal', '1e5j', False, False, 10, 1),
            Token('Complex Literal', '2.7E-12J', False, False, 11, 1),
            Token('Binary Literal', '0b1', True, False, 13, 1),
            Token('Identifier', 'e2', False, False, 13, 4),
            Token('Octal Literal', '0o5', True, False, 14, 1),
            Token('Identifier', 'e6', False, False, 14, 4),
            Token('Hex Literal', '0x2e4', False, False, 15, 1),
            Token('Zero Literal', '00', False, False, 16, 1),
            Token('Identifier', 'x3', False, False, 16, 3),
            Token('Hex Literal', '0x33213E', False, False, 17, 1),
            Token('Dec Int Literal', '-43213', False, False, 17, 9),
            Token('Dec Int Literal', '8', True, False, 19, 1),
            Token('Identifier', 'exj', False, False, 19, 2),
            Token('Exponent Literal', '12e2', False, False, 20, 1),
            Token('Identifier', 'ab', False, False, 20, 5)
        ])

    def test_extra_number_everything(self):
        tokens = self.token_list('''
            single
            +253232138821
            1.
            1e0
            1E-0
            .23
            0e0
            1.5E-5
            .5e2
            .0e+21
            .5e-12
            0Xdeadbeef
            10.J
            .001j
            1e100j
            3.14e-10j

            multiple
            1 .2
            1 e 5
            2 - -5
            7 + -8
            5 - +3
            8 - 2
            2++2

            recoverable errors
            1e+
            0o5j
            01
            -0189
            ''')

        self.assertEqual(tokens, [
            Token('Identifier', 'single', False, False, 1, 1),
            Token('Dec Int Literal', '+253232138821', False, False, 2, 1),
            Token('Float Literal', '1.', False, False, 3, 1),
            Token('Exponent Literal', '1e0', False, False, 4, 1),
            Token('Exponent Literal', '1E-0', False, False, 5, 1),
            Token('Float Literal', '.23', False, False, 6, 1),
            Token('Exponent Literal', '0e0', False, False, 7, 1),
            Token('Exponent Literal', '1.5E-5', False, False, 8, 1),
            Token('Exponent Literal', '.5e2', False, False, 9, 1),
            Token('Exponent Literal', '.0e+21', False, False, 10, 1),
            Token('Exponent Literal', '.5e-12', False, False, 11, 1),
            Token('Hex Literal', '0Xdeadbeef', False, False, 12, 1),
            Token('Complex Literal', '10.J', False, False, 13, 1),
            Token('Complex Literal', '.001j', False, False, 14, 1),
            Token('Complex Literal', '1e100j', False, False, 15, 1),
            Token('Complex Literal', '3.14e-10j', False, False, 16, 1),
            Token('Identifier', 'multiple', False, False, 18, 1),
            Token('Dec Int Literal', '1', False, False, 19, 1),
            Token('Float Literal', '.2', False, False, 19, 3),
            Token('Dec Int Literal', '1', False, False, 20, 1),
            Token('Identifier', 'e', False, False, 20, 3),
            Token('Dec Int Literal', '5', False, False, 20, 5),
            Token('Dec Int Literal', '2', False, False, 21, 1),
            Token('Arith Sign Operator', '-', False, False, 21, 3),
            Token('Dec Int Literal', '-5', False, False, 21, 5),
            Token('Dec Int Literal', '7', False, False, 22, 1),
            Token('Arith Sign Operator', '+', False, False, 22, 3),
            Token('Dec Int Literal', '-8', False, False, 22, 5),
            Token('Dec Int Literal', '5', False, False, 23, 1),
            Token('Arith Sign Operator', '-', False, False, 23, 3),
            Token('Dec Int Literal', '+3', False, False, 23, 5),
            Token('Dec Int Literal', '8', False, False, 24, 1),
            Token('Arith Sign Operator', '-', False, False, 24, 3),
            Token('Dec Int Literal', '2', False, False, 24, 5),
            Token('Dec Int Literal', '2', False, False, 25, 1),
            Token('Arith Sign Operator', '+', False, False, 25, 2),
            Token('Dec Int Literal', '+2', False, False, 25, 3),
            Token('Identifier', 'recoverable', False, False, 27, 1),
            Token('Identifier', 'errors', False, False, 27, 13),
            Token('Dec Int Literal', '1', True, False, 28, 1),
            Token('Identifier', 'e', False, False, 28, 2),
            Token('Arith Sign Operator', '+', False, False, 28, 3),
            Token('Octal Literal', '0o5', True, False, 29, 1),
            Token('Identifier', 'j', False, False, 29, 4),
            Token('Single Zero Literal', '0', True, False, 30, 1),
            Token('Dec Int Literal', '1', False, False, 30, 2),
            Token('Single Zero Literal', '-0', True, False, 31, 1),
            Token('Dec Int Literal', '189', False, False, 31, 3)
        ])


    def test_comment_multiple_pound_after_tokens_long(self):
        tokens = self.token_list('''
            # comment
               # 12352131 asasd
            ####
            #

            #x

            abc# x
            12312 3321      # o
            # _ [][ 432 432   ] {   }{ ? ??!~~~~`` ;; \ 3213   } asdha
            ''')

        self.assertEqual(tokens, [
            Token('Comment', '# comment', False, False, 1, 1),
            Token('Comment', '# 12352131 asasd', False, False, 2, 4),
            Token('Comment', '####', False, False, 3, 1),
            Token('Comment', '#', False, False, 4, 1),
            Token('Comment', '#x', False, False, 6, 1),
            Token('Identifier', 'abc', False, False, 8, 1),
            Token('Comment', '# x', False, False, 8, 4),
            Token('Dec Int Literal', '12312', False, False, 9, 1),
            Token('Dec Int Literal', '3321', False, False, 9, 7),
            Token('Comment', '# o', False, False, 9, 17),
            Token('Comment', '# _ [][ 432 432   ] {   }{ ? ??!~~~~`` ;; \\ 3213   } asdha', False, False, 10, 1)
        ])

    def test_string_literal_one_line_modifiers_single_double_quote_unrecoverable(self):
        tokens = self.token_list(r'''
            'abc'
            ''
               ""
            " "
            "123?!~"
            "fadjfasfasjffkdjfkfjsa"
            r'abc'
            U'bafadfas'
            u""
            '"hello" -me'
            "ooo 'iii' oo"
                  "   fadfasd fh adsjfhdsafh ads 'aaa'"
            ru'adaxas231'
            \n
            '\n'
            '
            '
            ''')

        self.assertEqual(tokens, [
            Token('Short String Literal', '\'abc\'', False, False, 1, 1),
            Token('Short String Literal', '\'\'', False, False, 2, 1),
            Token('Short String Literal', '""', False, False, 3, 4),
            Token('Short String Literal', '" "', False, False, 4, 1),
            Token('Short String Literal', '"123?!~"', False, False, 5, 1),
            Token('Short String Literal', '"fadjfasfasjffkdjfkfjsa"', False, False, 6, 1),
            Token('Short String Literal', 'r\'abc\'', False, False, 7, 1),
            Token('Short String Literal', 'U\'bafadfas\'', False, False, 8, 1),
            Token('Short String Literal', 'u""', False, False, 9, 1),
            Token('Short String Literal', '\'"hello" -me\'', False, False, 10, 1),
            Token('Short String Literal', '"ooo \'iii\' oo"', False, False, 11, 1),
            Token('Short String Literal', '"   fadfasd fh adsjfhdsafh ads \'aaa\'"', False, False, 12, 7),
            Token('Identifier', 'ru', False, False, 13, 1),
            Token('Short String Literal', '\'adaxas231\'', False, False, 13, 3),
            Token('Line Cont Delimiter', '\\', False, False, 14, 1),
            Token('Identifier', 'n', False, False, 14, 2),
            Token('Short String Literal', '\'\\n\'', False, False, 15, 1),
            Token('*Error*', '\'\n\'\n"...', False, True, 16, 1)
        ])

    def test_long_string_literal_single_double_quotes_modifiers_unrecoverable(self):
        # escaping hell...
        tokens = self.token_list('''
            \'\'\' abc \'\'\'

            \'\'\' \'\'\'

            """"""

            \'\'\'
            \'\'\'

            """

            """

            \'\'\'
            fadfasfa
            \'\'\'

            r"""dsak"""

            """
            # not comm
            """


            a \'\'\'
            inside
            \'\'\'b

            str = U"""
             hah
             bah
             dah """ x

            # unrecov
            \'\'\'
            ''')

        self.assertEqual(tokens, [
            Token('Long String Literal', '\'\'\' abc \'\'\'', False, False, 1, 1),
            Token('Long String Literal', '\'\'\' \'\'\'', False, False, 3, 1),
            Token('Long String Literal', '""""""', False, False, 5, 1),
            Token('Long String Literal', '\'\'\'\n\'\'\'', False, False, 7, 1),
            Token('Long String Literal', '"""\n\n"""', False, False, 10, 1),
            Token('Long String Literal', '\'\'\'\nfadfasfa\n\'\'\'', False, False, 14, 1),
            Token('Long String Literal', 'r"""dsak"""', False, False, 18, 1),
            Token('Long String Literal', '"""\n# not comm\n"""', False, False, 20, 1),
            Token('Identifier', 'a', False, False, 25, 1),
            Token('Long String Literal', '\'\'\'\ninside\n\'\'\'', False, False, 25, 3),
            Token('Identifier', 'b', False, False, 27, 4),
            Token('Identifier', 'str', False, False, 29, 1),
            Token('Assignment Operator', '=', False, False, 29, 5),
            Token('Long String Literal', 'U"""\n hah\n bah\n dah """', False, False, 29, 7),
            Token('Identifier', 'x', False, False, 32, 10),
            Token('Comment', '# unrecov', False, False, 34, 1),
            Token('*Error*', '\'\'\'\n\n...', False, True, 35, 1)
        ])

    def test_bytes_long_short_modifiers_double_single_one_three_quotes(self):
        tokens = self.token_list('''
            r """ """
            x""""""
            BR'~~'
            rb"""
            bin
            """
            b"""   """
            """ str """
            ''')

        self.assertEqual(tokens, [
            Token('Identifier', 'r', False, False, 1, 1),
            Token('Long String Literal', '""" """', False, False, 1, 3),
            Token('Identifier', 'x', False, False, 2, 1),
            Token('Long String Literal', '""""""', False, False, 2, 2),
            Token('Short Bytes Literal', 'BR\'~~\'', False, False, 3, 1),
            Token('Long Bytes Literal', 'rb"""\nbin\n"""', False, False, 4, 1),
            Token('Long Bytes Literal', 'b"""   """', False, False, 7, 1),
            Token('Long String Literal', '""" str """', False, False, 8, 1)
        ])

    def test_all_keywords(self):
        tokens = self.token_list('''
            if elif else
            for while
            break continue
            return yield assert pass
            raise try except finally
            def lambda class
            import from
            with as
            global nonlocal del
            ''')

        self.assertEqual(tokens, [
            Token('Decisional Keyword', 'if', False, False, 1, 1),
            Token('Decisional Keyword', 'elif', False, False, 1, 4),
            Token('Decisional Keyword', 'else', False, False, 1, 9),
            Token('Looping Keyword', 'for', False, False, 2, 1),
            Token('Looping Keyword', 'while', False, False, 2, 5),
            Token('Loop Control Keyword', 'break', False, False, 3, 1),
            Token('Loop Control Keyword', 'continue', False, False, 3, 7),
            Token('Flow Control Keyword', 'return', False, False, 4, 1),
            Token('Flow Control Keyword', 'yield', False, False, 4, 8),
            Token('Flow Control Keyword', 'assert', False, False, 4, 14),
            Token('Flow Control Keyword', 'pass', False, False, 4, 21),
            Token('Exceptions Keyword', 'raise', False, False, 5, 1),
            Token('Exceptions Keyword', 'try', False, False, 5, 7),
            Token('Exceptions Keyword', 'except', False, False, 5, 11),
            Token('Exceptions Keyword', 'finally', False, False, 5, 18),
            Token('Defining Keyword', 'def', False, False, 6, 1),
            Token('Defining Keyword', 'lambda', False, False, 6, 5),
            Token('Defining Keyword', 'class', False, False, 6, 12),
            Token('Importing Keyword', 'import', False, False, 7, 1),
            Token('Importing Keyword', 'from', False, False, 7, 8),
            Token('Aliasing Keyword', 'with', False, False, 8, 1),
            Token('Aliasing Keyword', 'as', False, False, 8, 6),
            Token('Var Ctrl Keyword', 'global', False, False, 9, 1),
            Token('Var Ctrl Keyword', 'nonlocal', False, False, 9, 8),
            Token('Var Ctrl Keyword', 'del', False, False, 9, 17)
        ])

    def test_all_delimiters(self):
        tokens = self.token_list('''
            ([]{})
            ... .. . .... ,, ;: - ->
            \\ \\\\ / \\\\\\
            ''')

        self.assertEqual(tokens, [
            Token('Grouping Delimiter', '(', False, False, 1, 1),
            Token('Grouping Delimiter', '[', False, False, 1, 2),
            Token('Grouping Delimiter', ']', False, False, 1, 3),
            Token('Grouping Delimiter', '{', False, False, 1, 4),
            Token('Grouping Delimiter', '}', False, False, 1, 5),
            Token('Grouping Delimiter', ')', False, False, 1, 6),
            Token('Punctuation Delimiter', '...', False, False, 2, 1),
            Token('Period Delimiter', '.', False, False, 2, 5),
            Token('Period Delimiter', '.', False, False, 2, 6),
            Token('Period Delimiter', '.', False, False, 2, 8),
            Token('Punctuation Delimiter', '...', False, False, 2, 10),
            Token('Period Delimiter', '.', False, False, 2, 13),
            Token('Punctuation Delimiter', ',', False, False, 2, 15),
            Token('Punctuation Delimiter', ',', False, False, 2, 16),
            Token('Punctuation Delimiter', ';', False, False, 2, 18),
            Token('Punctuation Delimiter', ':', False, False, 2, 19),
            Token('Arith Sign Operator', '-', False, False, 2, 21),
            Token('Punctuation Delimiter', '->', False, False, 2, 23),
            Token('Line Cont Delimiter', '\\', False, False, 3, 1),
            Token('Line Cont Delimiter', '\\', False, False, 3, 3),
            Token('Line Cont Delimiter', '\\', False, False, 3, 4),
            Token('Arithmetic Operator', '/', False, False, 3, 6),
            Token('Line Cont Delimiter', '\\', False, False, 3, 8),
            Token('Line Cont Delimiter', '\\', False, False, 3, 9),
            Token('Line Cont Delimiter', '\\', False, False, 3, 10)
        ])

    def test_all_operators(self):
        tokens = self.token_list('''
            + - +- ++
            * ** ***
            // / /// % @ %@%
            += -+= /=/
            & | ^ v << >>
            <>< <<> </> && <=>
            == != !== === ><
            in not in init not   in int
            is isint is not   not is is in is is in in
            = &= &== |= ^= ^^ <<= >>= >=<
            or and orand rando andor andand aaand
            not is is not isnot notis is   not not    is is not not in is is

            !
            ''')

        self.assertEqual(tokens, [
            Token('Arith Sign Operator', '+', False, False, 1, 1),
            Token('Arith Sign Operator', '-', False, False, 1, 3),
            Token('Arith Sign Operator', '+', False, False, 1, 5),
            Token('Arith Sign Operator', '-', False, False, 1, 6),
            Token('Arith Sign Operator', '+', False, False, 1, 8),
            Token('Arith Sign Operator', '+', False, False, 1, 9),
            Token('Arithmetic Operator', '*', False, False, 2, 1),
            Token('Arithmetic Operator', '**', False, False, 2, 3),
            Token('Arithmetic Operator', '**', False, False, 2, 6),
            Token('Arithmetic Operator', '*', False, False, 2, 8),
            Token('Arithmetic Operator', '//', False, False, 3, 1),
            Token('Arithmetic Operator', '/', False, False, 3, 4),
            Token('Arithmetic Operator', '//', False, False, 3, 6),
            Token('Arithmetic Operator', '/', False, False, 3, 8),
            Token('Arithmetic Operator', '%', False, False, 3, 10),
            Token('Arithmetic Operator', '@', False, False, 3, 12),
            Token('Arithmetic Operator', '%', False, False, 3, 14),
            Token('Arithmetic Operator', '@', False, False, 3, 15),
            Token('Arithmetic Operator', '%', False, False, 3, 16),
            Token('Arith Assign Operator', '+=', False, False, 4, 1),
            Token('Arith Sign Operator', '-', False, False, 4, 4),
            Token('Arith Assign Operator', '+=', False, False, 4, 5),
            Token('Arith Assign Operator', '/=', False, False, 4, 8),
            Token('Arithmetic Operator', '/', False, False, 4, 10),
            Token('Binary Bit-Wise Operator', '&', False, False, 5, 1),
            Token('Binary Bit-Wise Operator', '|', False, False, 5, 3),
            Token('Binary Bit-Wise Operator', '^', False, False, 5, 5),
            Token('Identifier', 'v', False, False, 5, 7),
            Token('Binary Bit-Wise Operator', '<<', False, False, 5, 9),
            Token('Binary Bit-Wise Operator', '>>', False, False, 5, 12),
            Token('Relational Operator', '<', False, False, 6, 1),
            Token('Relational Operator', '>', False, False, 6, 2),
            Token('Relational Operator', '<', False, False, 6, 3),
            Token('Binary Bit-Wise Operator', '<<', False, False, 6, 5),
            Token('Relational Operator', '>', False, False, 6, 7),
            Token('Relational Operator', '<', False, False, 6, 9),
            Token('Arithmetic Operator', '/', False, False, 6, 10),
            Token('Relational Operator', '>', False, False, 6, 11),
            Token('Binary Bit-Wise Operator', '&', False, False, 6, 13),
            Token('Binary Bit-Wise Operator', '&', False, False, 6, 14),
            Token('Relational Operator', '<=', False, False, 6, 16),
            Token('Relational Operator', '>', False, False, 6, 18),
            Token('Relational Operator', '==', False, False, 7, 1),
            Token('Relational Operator', '!=', False, False, 7, 4),
            Token('Relational Operator', '!=', False, False, 7, 7),
            Token('Assignment Operator', '=', False, False, 7, 9),
            Token('Relational Operator', '==', False, False, 7, 11),
            Token('Assignment Operator', '=', False, False, 7, 13),
            Token('Relational Operator', '>', False, False, 7, 15),
            Token('Relational Operator', '<', False, False, 7, 16),
            Token('Rel Containment Operator', 'in', False, False, 8, 1),
            Token('Rel Containment Operator', 'not in', False, False, 8, 4),
            Token('Identifier', 'init', False, False, 8, 11),
            Token('Rel Containment Operator', 'not   in', False, False, 8, 16),
            Token('Identifier', 'int', False, False, 8, 25),
            Token('Identity Operator', 'is', False, False, 9, 1),
            Token('Identifier', 'isint', False, False, 9, 4),
            Token('Neg Identity Operator', 'is not', False, False, 9, 10),
            Token('Logical Neg Operator', 'not', False, False, 9, 19),
            Token('Identity Operator', 'is', False, False, 9, 23),
            Token('Identity Operator', 'is', False, False, 9, 26),
            Token('Rel Containment Operator', 'in', False, False, 9, 29),
            Token('Identity Operator', 'is', False, False, 9, 32),
            Token('Identity Operator', 'is', False, False, 9, 35),
            Token('Rel Containment Operator', 'in', False, False, 9, 38),
            Token('Rel Containment Operator', 'in', False, False, 9, 41),
            Token('Assignment Operator', '=', False, False, 10, 1),
            Token('Bit-Wise Assign Operator', '&=', False, False, 10, 3),
            Token('Bit-Wise Assign Operator', '&=', False, False, 10, 6),
            Token('Assignment Operator', '=', False, False, 10, 8),
            Token('Bit-Wise Assign Operator', '|=', False, False, 10, 10),
            Token('Bit-Wise Assign Operator', '^=', False, False, 10, 13),
            Token('Binary Bit-Wise Operator', '^', False, False, 10, 16),
            Token('Binary Bit-Wise Operator', '^', False, False, 10, 17),
            Token('Bit-Wise Assign Operator', '<<=', False, False, 10, 19),
            Token('Bit-Wise Assign Operator', '>>=', False, False, 10, 23),
            Token('Relational Operator', '>=', False, False, 10, 27),
            Token('Relational Operator', '<', False, False, 10, 29),
            Token('Logical Operator', 'or', False, False, 11, 1),
            Token('Logical Operator', 'and', False, False, 11, 4),
            Token('Identifier', 'orand', False, False, 11, 8),
            Token('Identifier', 'rando', False, False, 11, 14),
            Token('Identifier', 'andor', False, False, 11, 20),
            Token('Identifier', 'andand', False, False, 11, 26),
            Token('Identifier', 'aaand', False, False, 11, 33),
            Token('Logical Neg Operator', 'not', False, False, 12, 1),
            Token('Identity Operator', 'is', False, False, 12, 5),
            Token('Neg Identity Operator', 'is not', False, False, 12, 8),
            Token('Identifier', 'isnot', False, False, 12, 15),
            Token('Identifier', 'notis', False, False, 12, 21),
            Token('Neg Identity Operator', 'is   not', False, False, 12, 27),
            Token('Logical Neg Operator', 'not', False, False, 12, 36),
            Token('Identity Operator', 'is', False, False, 12, 43),
            Token('Neg Identity Operator', 'is not', False, False, 12, 46),
            Token('Rel Containment Operator', 'not in', False, False, 12, 53),
            Token('Identity Operator', 'is', False, False, 12, 60),
            Token('Identity Operator', 'is', False, False, 12, 63),
            Token('*Error*', '!\\n', False, True, 14, 1)
        ])

    def test_additional_single_line_string(self):
        tokens = self.token_list(r'''
            "   "
            faokeofka ""
            '"'
            "inside is ' ' blank"
            r'abc'
            R"bacd "
            :;.@#fadfas " double quotes   "
               'str' # comm
            r''

            unrecoverable
            'a
            b'
            '
            "

            "
            '
            '''
            '''
            '"'
            ''')

        self.assertEqual(tokens, [
            Token('Short String Literal', '"   "', False, False, 1, 1),
            Token('Identifier', 'faokeofka', False, False, 2, 1),
            Token('Short String Literal', '""', False, False, 2, 11),
            Token('Short String Literal', '\'"\'', False, False, 3, 1),
            Token('Short String Literal', '"inside is \' \' blank"', False, False, 4, 1),
            Token('Short String Literal', 'r\'abc\'', False, False, 5, 1),
            Token('Short String Literal', 'R"bacd "', False, False, 6, 1),
            Token('Punctuation Delimiter', ':', False, False, 7, 1),
            Token('Punctuation Delimiter', ';', False, False, 7, 2),
            Token('Period Delimiter', '.', False, False, 7, 3),
            Token('Arithmetic Operator', '@', False, False, 7, 4),
            Token('Comment', '#fadfas " double quotes   "', False, False, 7, 5),
            Token('Short String Literal', '\'str\'', False, False, 8, 4),
            Token('Comment', '# comm', False, False, 8, 10),
            Token('Short String Literal', 'r\'\'', False, False, 9, 1),
            Token('Identifier', 'unrecoverable', False, False, 11, 1),
            Token('*Error*', '\'a\nb\'...', False, True, 12, 1)
        ])

    # Python source does not support emojis
    # def test_illegal_chars_non_unicode(self):
    #     tokens = self.token_list(r'''
    #         # ?!`
    #         # →
    #         # 🐶
    #         ?
    #         !
    #         `
    #         →
    #         🍺
    #         ''')
    #
    #     self.assertEqual(tokens, [
    #         Token('Comment', '# ?!`', False, False, 1, 1),
    #         Token('Comment', '# ', False, False, 2, 1),
    #         Token('*Error*', '→\n# 🐶...', False, True, 2, )
    #     ])


if __name__ == '__main__':
    unittest.main()

