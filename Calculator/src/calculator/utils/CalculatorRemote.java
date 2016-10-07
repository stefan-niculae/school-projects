package calculator.utils;


import java.rmi.Remote;
import java.rmi.RemoteException;


public interface CalculatorRemote extends Remote
{
    // Binary operations
    double add      (double x, double y)        throws RemoteException;
    double subtract (double x, double y)        throws RemoteException;
    double multiply (double x, double y)        throws RemoteException;
    double divide   (double x, double y)        throws RemoteException;
    double raise    (double base, double exp)   throws RemoteException;

    // Unary operations
    double squareRoot   (double x) throws RemoteException;
    double inverse      (double x) throws RemoteException;
    double factorial    (double x) throws RemoteException;

    // Memory operations
    void setMem     (double x)  throws RemoteException;
    double getMem   ()          throws RemoteException;
    void addToMem   (double x)  throws RemoteException;
    void subFromMem (double x)  throws RemoteException;
}
