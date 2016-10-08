package contacts;

import java.sql.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;


class DBConnectivity {
    private static final String MYSQL_DRIVER = "com.mysql.jdbc.Driver";
    private static final String MYSQL_URL_PREFIX = "jdbc:mysql://";

    private static final String DB_ADDRESS = "localhost";
    private static final String DB_PORT = "3306";

    private static final String DB_NAME = "Contacts";
    private static final String DB_USER = "root";
    private static final String DB_PASSWORD = "";

    private static DBConnectivity instance = null;

    static DBConnectivity getInstance() {
        if (instance == null)
            instance = new DBConnectivity();
        return instance;
    }

    private Connection connection;
    private Statement statement;

    //region Connection Handling
    private DBConnectivity() {
        // Connecting to the database
        LoadDriver();

        ConnectTo(MYSQL_URL_PREFIX + DB_ADDRESS + ":" + DB_PORT + "/" + DB_NAME,
                DB_USER,
                DB_PASSWORD);
    }

    private void LoadDriver() {
        try {
            Class.forName(MYSQL_DRIVER);
        } catch (ClassNotFoundException e) {
            System.err.print("Could not load driver.\n");
            e.printStackTrace();
        }
    }

    private void ConnectTo(String url, String user, String password) {
        try {
            connection = DriverManager.getConnection(url, user, password);
        } catch (SQLException e) {
            System.err.printf("Could not connect to %s as %s (pw: '%s').\n", url, user, password);
            e.printStackTrace();
        }
    }
    //endregion

    private static HashMap<String, String> DBcolumn = new HashMap<>();

    static {
        DBcolumn.put("ID",          "id");
        DBcolumn.put("Name",        "prenume");
        DBcolumn.put("Surname",     "nume");
        DBcolumn.put("Phone",       "tel_mobil");
        DBcolumn.put("Other phone", "tel_fix");
        DBcolumn.put("Email",       "email");
        DBcolumn.put("Address",     "adresa");
        DBcolumn.put("City",        "oras");
        DBcolumn.put("County",      "judet");
        DBcolumn.put("Postal code", "cod_postal");
    }

    //region CRUD

    /**
     * @return error description upon failure, null on success
     */
    String Create(Contact contact) {
        // This will never happen because the respective fields have the 'required' property
        if (!contact.fieldsOk())
            return "Name, surname, phone and email are required";

        try {
            statement = connection.createStatement();
        } catch (SQLException e) {
            e.printStackTrace();
            return "Could not create statement";
        }

        //language=MySQL
        String insert =
                "INSERT INTO Persoane (nume, prenume, tel_mobil, tel_fix, email, adresa, oras, judet, cod_postal) " +
                "VALUES              " + contact.asSQLList() + ";";

        try {
            int result = statement.executeUpdate(insert);
            if (result == 0)
                return String.format("Could not insert %s", insert);

        } catch (SQLException e) {
            e.printStackTrace();
            return "Could not execute update";
        }

        return null;
    }

    List<Contact> Read(String column, String condition, StringBuilder errorDescription) {
        List<Contact> results = new ArrayList<>();
        errorDescription.setLength(0); // clear the sb

        try {
            statement = connection.createStatement();
        } catch (SQLException e) {
            e.printStackTrace();
            errorDescription.append("Could not create statement");
            return null;
        }

        //language=MySQL
        String select =
                "SELECT * " +
                "FROM   Persoane ";

        if (!condition.isEmpty())
            select +=
                    "WHERE  " + DBcolumn.get(column) + " = '" + condition + "'";

        select +=
                ";";


        ResultSet resultSet;
        try {
            resultSet = statement.executeQuery(select);
        } catch (SQLException e) {
            e.printStackTrace();
            errorDescription.append(String.format("Could not execute %s", select));
            return null;
        }

        try {
            while (resultSet.next())
                results.add(new Contact(
                        resultSet.getString("id"),
                        resultSet.getString("prenume"),
                        resultSet.getString("nume"),
                        resultSet.getString("tel_mobil"),
                        resultSet.getString("tel_fix"),
                        resultSet.getString("email"),
                        resultSet.getString("adresa"),
                        resultSet.getString("oras"),
                        resultSet.getString("judet"),
                        resultSet.getString("cod_postal")
                ));

        } catch (SQLException e) {
            e.printStackTrace();
            errorDescription.append("Could not read results");
            return null;
        }

        return results;
    }

    String Update(Contact contact) {
        try {
            statement = connection.createStatement();
        } catch (SQLException e) {
            e.printStackTrace();
            return "Could not create statement";
        }

        //language=MySQL
        String update =
                "UPDATE     Persoane " +
                "SET         nume = " +         Contact.SQLSurrounded(contact.surname) + "," +
                            "prenume = " +      Contact.SQLSurrounded(contact.name) + "," +
                            "tel_mobil = " +    Contact.SQLSurrounded(contact.phone) + "," +
                            "tel_fix = " +      Contact.SQLSurrounded(contact.otherPhone) + "," +
                            "email = " +        Contact.SQLSurrounded(contact.email) + "," +
                            "adresa = " +       Contact.SQLSurrounded(contact.address) + "," +
                            "oras = " +         Contact.SQLSurrounded(contact.city) + "," +
                            "judet = " +        Contact.SQLSurrounded(contact.county) + "," +
                            "cod_postal = " +   Contact.SQLSurrounded(contact.postalCode) + " " +
                "WHERE      id = '" + contact.ID + "'";

        try {
            int result = statement.executeUpdate(update);
            if (result == 0)
                return String.format("The entry's id (%s) was changed while trying to edit", contact.ID);

        } catch (SQLException e) {
            e.printStackTrace();
            return "Could not execute update";
        }

        return null;
    }

    String Delete(String id) {
        try {
            statement = connection.createStatement();
        } catch (SQLException e) {
            e.printStackTrace();
            return "Could not create statement";
        }

        //language=MySQL
        String delete =
                "DELETE " +
                        "FROM   Persoane " +
                        "WHERE  id = " + id + ";";

        try {
            int result = statement.executeUpdate(delete);
            if (result == 0)
                return String.format("ID %s does not exist", id);

        } catch (SQLException e) {
            e.printStackTrace();
            return "Could not execute update";
        }

        return null;
    }
    //endregion

}
