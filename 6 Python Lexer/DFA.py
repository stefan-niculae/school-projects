def which_of(x, elems):
    for elem in elems:
        if x.startswith(elem):
            return elem

def predicate_into_grabbing(pred):
    return lambda x: x[0] if pred(x[0]) else None

class Transition:
    def __init__(self, condition, to_state_name):
        # The condition is a predicate
        # But if a list or a string is received, it means the predicate
        # is whether that element is in it
        if isinstance(condition, list) or isinstance(condition, str):
            self.matching_list = condition
            self.condition = lambda s: which_of(s, self.matching_list)
        else:
            self.matching_list = None
            self.condition = predicate_into_grabbing(condition)
        self.to_state_name = to_state_name

    def __str__(self):
        return '---{}---> {}'.format(
            self.matching_list if self.matching_list else self.condition.__name__,
            self.to_state_name)

class State:
    def __init__(self, name=None, transitions=[], is_final=False):
        self.name = name
        # A state can have a single transitions or a list of them
        if not isinstance(transitions, list):
            transitions = [transitions]
        self.transitions = transitions
        self.is_final = is_final

    def __str__(self):
        trans = map(str, self.transitions)
        return '{}[{}]: {}'.format(
            self.name,
            'F' if self.is_final else 'N',
            '\n  '.join(trans) if len(self.transitions) > 0 else '-')

class DFA:
    def __init__(self, states):
        for state_name in states:
            states[state_name].name = state_name
        self.states = states

    def __str__(self):
        return '\n'.join(map(str, self.states.values()))

    def run_on(self, inp_str):
        curr_state = self.states['start']

        last_acc_state = None
        last_acc_idx = None

        # Run the DFA util it stops
        i = 0
        while i < len(inp_str):

            # Try each transition, in order
            for trans in curr_state.transitions:
                matched_chunk = trans.condition(inp_str[i:])

                if matched_chunk:
                    i += len(matched_chunk)
                    next_state = self.states[trans.to_state_name]
                    curr_state = next_state

                    # Remember the last accepting state
                    if next_state.is_final:
                        last_acc_state = next_state
                        last_acc_idx = i

                    # Stop after finding the first matching state
                    break

            # If no transition could be made (or if none exists), means we stopped
            if not matched_chunk or len(curr_state.transitions) == 0:
                break

        if last_acc_state and last_acc_state != curr_state:
            stop_state = last_acc_state
            index = last_acc_idx
            recovered = True
        else:
            stop_state = curr_state
            index = i
            recovered = False

        return stop_state, index, recovered

