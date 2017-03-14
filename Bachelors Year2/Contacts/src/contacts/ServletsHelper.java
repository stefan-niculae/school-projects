package contacts;


import javax.servlet.http.HttpServletResponse;
import java.io.IOException;


class ServletsHelper {
    //language=HTML
    private static String navBar =
            "<nav>" +
                    "<a href='/'><img src='/images/home.png'></a>" +
                    "<a href='/add'><img src='/images/add.png'></a>" +
                    "<a href='/edit'><img src='/images/edit.png'></a>" +
                    "<a href='/delete'><img src='/images/delete.png'></a>" +
                    "<a href='/search'><img src='/images/search.png'></a>" +
            "</nav>";

    static String wordsPattern =
            "pattern = '[A-Za-z]+(\\s([A-Za-z])+)*'";
    static String digitsPattern =
            "pattern = '\\d+'";
    static String wordsWithDigitsPattern =
            "pattern = '[A-Za-z0-9]+(\\s([A-Za-z0-9])+)*'";

    static void writeDocument(HttpServletResponse response, String[] stylesheets, String body) throws IOException {
        String sheets = "";
        for (String sheet : stylesheets)
            //language=HTML
            sheets += "<link rel='stylesheet' href='CSS/" + sheet + "'>";

        //language=HTML
        String page =
                "<head>" +
                        "<link rel='stylesheet' href='CSS/navbar.css'>" +
                        "<link rel='stylesheet' href='CSS/content-style.css'>" +
                        sheets +
                "</head>" +
                "<body>"+
                        navBar +
                        body +
                "</body>";

        response.getWriter().println(page);
    }

    static String generateConfirmation(String errorDescription, String successMessage) {
        //language=HTML
        String confirmation =
                "<section class='confirmation'>";

        // No error
        if (errorDescription == null)
            confirmation +=
                    "<p class='success'>" + successMessage + "</p>";
        else
            confirmation +=
                    "<p class='failure'>" + errorDescription + "</p>";


        confirmation +=
                "</section>";

        return confirmation;
    }
}
