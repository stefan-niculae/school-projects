package contacts;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;


@WebServlet(name = "AddServlet")
public class AddServlet extends HttpServlet {
    private String editStyle = "edit-style.css";

    //language=HTML
    private String title =
            "<header>Add Contact</header>";

    //language=HTML
    private String addForm =
            "<form method='post' class='edit'>" +
                    // We add a pattern validation to each field to combat SQL Injection
                    "<input type='text' name='name-field'       placeholder='Name'          required "
                    + ServletsHelper.wordsPattern + ">" +
                    "<input type='text' name='surname-field'    placeholder='Surname'       required "
                    + ServletsHelper.wordsPattern + ">" +

                    "<input type='text' name='phone-field'      placeholder='Phone'         required " +
                    "pattern='07[0-9]{8}' title='Enter a phone number starting with 07 with 8 following digits'>" +
                    "<input type='text' name='other-phone-field' placeholder='Other phone' " +
                    "pattern='07[0-9]{8}' title='Enter a phone number starting with 07 with eight following digits'>" +

                    "<input type='email' name='email-field'      placeholder='Email'         required " +
                    // Characters followed by an @ sign, followed by more characters, and then a "."
                    // After the "." sign, you can only write 2 to 3 letters from a to z
                    "pattern='[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,3}$' title='Enter a valid email address'>" +

                    "<input type='text' name='address-field'    placeholder='Address' "
                    + ServletsHelper.wordsPattern + ">" +
                    "<input type='text' name='city-field'       placeholder='City' "
                    + ServletsHelper.wordsPattern + ">" +
                    "<input type='text' name='county-field'     placeholder='County' "
                    + ServletsHelper.wordsPattern + ">" +

                    "<input type='text' name='postal-code-field' placeholder='Postal code' " +
                    "pattern='[0-9]{6}' title='Enter a six digit postal code'>" +

                    "<input type='submit' value='Add'>" +
            "</form>";

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws
                                                                                    ServletException,
                                                                                    IOException {
        Contact entered = new Contact(
                "-1", // id is auto incremented
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

        String errorDescription = DBConnectivity.getInstance().Create(entered);

        String confirmation = ServletsHelper.generateConfirmation(errorDescription,
                "Successfully added");

        ServletsHelper.writeDocument(response,
                new String[]{editStyle},

                title +
                        addForm +
                        confirmation);
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws
            ServletException,
            IOException {
        ServletsHelper.writeDocument(response, new String[]{editStyle}, title + addForm);
    }

}
