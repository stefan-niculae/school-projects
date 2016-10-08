package contacts;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.List;


@WebServlet (name = "SearchServlet")
public class SearchServlet extends HttpServlet
{
    private String searchStyle = "search-style.css";

    //language=HTML
    private String title =
            "<header>Search Contacts</header>";

    //language=HTML
    private String searchForm =
            "<form method='post' class='search'>" +
                    "<select name='search-criteria'>" +
                        "<option name='id-option'>ID</option>" +
                        "<option name='name-option'>Name</option>" +
                        "<option name='surname-option'>Surname</option>" +
                        "<option name='phone-option'>Phone</option>" +
                        "<option name='other-phone-option'>Other phone</option>" +
                        "<option name='email-option'>Email</option>" +
                        "<option name='address-option'>Address</option>" +
                        "<option name='city-option'>City</option>" +
                        "<option name='county-option'>County</option>" +
                        "<option name='postal-code-option'>Postal code</option>" +
                    "</select>" +
                    "<input type='text' name='search-field' placeholder='Criteria' class='small' "
                    + ServletsHelper.wordsWithDigitsPattern + ">" +
                    "<input type='submit' value='Search' class='small'>" +
            "</form>";

    protected void doPost (HttpServletRequest request, HttpServletResponse response) throws
                                                                                     ServletException,
                                                                                     IOException {
        String column = request.getParameter("search-criteria");
        String condition = request.getParameter("search-field");

        // We use a string builder because there are no pass by reference options in java
        StringBuilder errorDescription = new StringBuilder();
        List<Contact> searchResults = DBConnectivity.getInstance().Read(column, condition, errorDescription);

        String resultsTable;

        // Error occurred
        if (errorDescription.length() != 0)
            resultsTable = ServletsHelper.generateConfirmation(errorDescription.toString(),
                    null);

        // No results to show
        else if (searchResults.isEmpty())
            resultsTable = ServletsHelper.generateConfirmation(
                     String.format("No entry has %s as %s", condition, column),
                    null);

        else {

            //language=HTML
            resultsTable = "<table id='search-results'>" +
                    "<tr>" +
                    "<th>ID</th>" +
                    "<th>Name</th>" +
                    "<th>Surname</th>" +
                    "<th>Phone</th>" +
                    "<th>Other Phone</th>" +
                    "<th>Email</th>" +
                    "<th>Address</th>" +
                    "<th>City</th>" +
                    "<th>County</th>" +
                    "<th>Postal Code</th>" +
                    "</tr>";

            boolean alternativeRow = false;
            for (Contact contact : searchResults) {
                String trTag = "<tr>";
                if (alternativeRow) trTag = "<tr class = 'alt'>";
                alternativeRow = !alternativeRow;

                //language=HTML
                resultsTable += trTag +
                        contact.asTableRow() +
                        "</tr>";
            }


            resultsTable += "</table>";
        }

        ServletsHelper.writeDocument(response,
                new String[]{searchStyle},
                title +
                        searchForm +
                        resultsTable);

    }
    
    protected void doGet (HttpServletRequest request, HttpServletResponse response) throws
                                                                                    ServletException,
                                                                                    IOException {
        ServletsHelper.writeDocument(response,
                new String[]{searchStyle},
                title +
                        searchForm);
    }
}
