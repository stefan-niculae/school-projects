package calculator.server;


import calculator.utils.Constants;

import java.rmi.AlreadyBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;


public class Server {
    public static void main(String[] args) throws RemoteException, AlreadyBoundException {
        Calculator calculator = new Calculator();

        Registry registry = LocateRegistry.createRegistry(Constants.PORT);
        registry.bind(Constants.ID, calculator);

        System.out.println("> Server started");
    }
}
