package contacts;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.List;


@WebServlet (name = "EditServlet")
public class EditServlet extends HttpServlet
{
    private String searchStyle = "search-style.css";
    private String editStyle = "edit-style.css";

    //language=HTML
    private String title =
            "<header>Edit Contact</header>";

    //language=HTML
    private String searchForm =
            "<form method='post' class='search'>" +
                    "<input type='text' name='id-field' placeholder='ID' value='{}' required "
                    + ServletsHelper.digitsPattern + " title='Enter a valid ID'>" +
                    "<input type='submit' value='Find'>"+
            "</form>";

    //language=HTML
    private static String editForm =
            "<form method='post' class='edit' style='display: {display-type}'>" +
                    "<input type='text' name='hidden-id-field'                              value='{hidden-id-value}' style='display: none;'>" +

                    "<input type='text' name='name-field'       placeholder='Name'          value='name-value'          required "
                    + ServletsHelper.wordsPattern + ">" +
                    "<input type='text' name='surname-field'    placeholder='Surname'       value='surname-value'       required "
                    + ServletsHelper.wordsPattern + ">" +

                    "<input type='text' name='phone-field'      placeholder='Phone'         value='phone-value'         required " +
                    "pattern='07[0-9]{8}' title='Enter a phone number starting with 07 with 8 following digits'>" +
                    "<input type='text' name='other-phone-field' placeholder='Other phone'   value='other-phone-value' " +
                    "pattern='07[0-9]{8}' title='Enter a phone number starting with 07 with eight following digits'>" +

                    "<input type='email' name='email-field'      placeholder='Email'         value='email-value'         required " +
                    "pattern='[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,3}$' title='Enter a valid email address'>" +

                    "<input type='text' name='address-field'    placeholder='Address'       value='address-value' "
                    + ServletsHelper.wordsPattern + ">" +
                    "<input type='text' name='city-field'       placeholder='City'          value='city-value' "
                    + ServletsHelper.wordsPattern + ">" +
                    "<input type='text' name='county-field'     placeholder='County'        value='county-value' "
                    + ServletsHelper.wordsPattern + ">" +

                    "<input type='text' name='postal-code-field' placeholder='Postal code'   value='postal-code-value' " +
                    "pattern='[0-9]{6}' title='Enter a six digit postal code'>" +


                    "<input type='submit' value='Edit'>" +
            "</form>";


    protected void doPost (HttpServletRequest request, HttpServletResponse response) throws
                                                                                     ServletException,
                                                                                     IOException {
        String id = request.getParameter("id-field");
        String populatedForm;

        // Search phase
        if (id != null && !id.isEmpty()) {
            StringBuilder searchError = new StringBuilder();

            List<Contact> matched = DBConnectivity.getInstance().Read("ID", id, searchError);

            if (searchError.length() != 0)
                populatedForm = ServletsHelper.generateConfirmation(searchError.toString(), null);
            else {
                if (matched.isEmpty())
                    populatedForm = ServletsHelper.generateConfirmation(String.format("No entry found for ID %s", id), null);
                else {
                    Contact searchedContact = matched.get(0); // id is PK => it is unique => if it isn't empty, there must be exactly ONE

                    String form = editForm.replace("{display-type}", "visible");
                    form = form.replace("{hidden-id-value}", id);
                    populatedForm = searchedContact.fillForm(form);
                }
            }
        }

        // Edit phase
        else {
            Contact entered = new Contact(
                    request.getParameter("hidden-id-field"),
                    request.getParameter("name-field"),
                    request.getParameter("surname-field"),
                    request.getParameter("phone-field"),
                    request.getParameter("other-phone-field"),
                    request.getParameter("email-field"),
                    request.getParameter("address-field"),
                    request.getParameter("city-field"),
                    request.getParameter("county-field"),
                    request.getParameter("postal-code-field")
            );
            String errorDescription = DBConnectivity.getInstance().Update(entered);
            populatedForm = ServletsHelper.generateConfirmation(errorDescription, "Edit successful");
        }


        String filledSearchForm = searchForm.replace("{}", id == null ? "" : id);

        ServletsHelper.writeDocument(response,
                new String[]{editStyle, searchStyle},
                title +
                        filledSearchForm +
                        populatedForm);
    }
    
    protected void doGet (HttpServletRequest request, HttpServletResponse response) throws
                                                                                    ServletException,
                                                                                    IOException {
        String emptySearchForm = searchForm.replace("{}", "");

        ServletsHelper.writeDocument(response,
                new String[]{editStyle, searchStyle},
                title +
                        emptySearchForm);
    }
}
