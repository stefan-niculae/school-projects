package calculator.server;

import calculator.utils.CalculatorRemote;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;


public class Calculator extends UnicastRemoteObject implements CalculatorRemote {
    private double memory;

    protected Calculator() throws RemoteException {
        super();
    }

    //region Binary Operations
    @Override
    public double add(double x, double y) {
        return x + y;
    }

    @Override
    public double subtract(double x, double y) {
        return x - y;
    }

    @Override
    public double multiply(double x, double y) {
        return x * y;
    }

    @Override
    public double divide(double x, double y) throws ArithmeticException {
        return x / y;
    }

    @Override
    public double raise(double base, double exp) throws ArithmeticException {
        return Math.pow(base, exp);
    }
    //endregion

    //region Unary Operations
    @Override
    public double squareRoot(double x) throws ArithmeticException {
        return Math.sqrt(x);
    }

    @Override
    public double inverse(double x) throws ArithmeticException {
        return 1 / x;
    }


    @Override
    public double factorial(double x) throws ArithmeticException {
        if (x < 0 || (int) x != x || x > 60)
            throw new ArithmeticException();

        double copy = x;
        double result = 1;
        while (copy > 1) {
            result *= copy;
            copy--;
        }

        return result;
    }
    //endregion

    //region Memory Operations
    @Override
    public void setMem(double x) {
        memory = x;
    }

    @Override
    public double getMem() {
        return memory;
    }

    @Override
    public void addToMem(double x) throws RemoteException {
        memory += x;
    }

    @Override
    public void subFromMem(double x) throws RemoteException {
        memory -= x;
    }
    //endregion
}
