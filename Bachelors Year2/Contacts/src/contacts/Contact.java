package contacts;


class Contact
{
    String ID;
    String name;
    String surname;
    String phone;
    String otherPhone;
    String email;
    String address;
    String city;
    String county;
    String postalCode;

    Contact(String ID, String name, String surname, String phone, String otherPhone, String email, String
            address, String city, String county, String postalCode) {
        this.ID         = ID;
        this.name       = name;
        this.surname    = surname;
        this.phone      = phone;
        this.otherPhone = emptyStringIfNull (otherPhone);
        this.email      = email;
        this.address    = emptyStringIfNull (address);
        this.city       = emptyStringIfNull (city);
        this.county     = emptyStringIfNull (county);
        this.postalCode = emptyStringIfNull (postalCode);
    }

    // Poor old Java doesn't have an NVL operator
    private String emptyStringIfNull(String string) {
        return string == null ? "" : string;
    }

    static final Contact dummyContact = new Contact ("24", "Stefan", "Niculae", "0722740468", null, "stefanconturi@gmail.com",
                                                     null, "Bucharest", null, null);

    String fillForm(String form) {
        String result = form;

        result = result.replace ("surname-value", surname); // Replace surname before the 'name' because is contained in 'surname'
        result = result.replace ("name-value", name);
        result = result.replace ("other-phone-value", otherPhone); // Same for 'other-phone' into 'phone'
        result = result.replace ("phone-value", phone);
        result = result.replace ("email-value", email);
        result = result.replace ("address-value", address);
        result = result.replace ("city-value", city);
        result = result.replace ("county-value", county);
        result = result.replace ("postal-code-value", postalCode);

        return result;
    }

    String asTableRow() {
        String result = "";

        //language=HTML
        result += "<td>"+ ID +          "</td>";
        result += "<td>"+ name +        "</td>";
        result += "<td>"+ surname +     "</td>";
        result += "<td>"+ phone +       "</td>";
        result += "<td>"+ otherPhone +  "</td>";
        result += "<td>"+ email +       "</td>";
        result += "<td>"+ address +     "</td>";
        result += "<td>"+ city +        "</td>";
        result += "<td>"+ county +      "</td>";
        result += "<td>"+ postalCode +  "</td>";

        return result;
    }

    @Override
    public String toString () {
        return String.format("id=%s,name=%s,surname=%s,phone=%s,otherphone=%s,email=%s,address=%s,city=%s,county=%s,postal=%s",
                             ID,    name,   surname,   phone,   otherPhone,   email,   address,   city,   county,   postalCode);
    }

    String asSQLList() {
        return String.format (
                // no id
                "(%s, %s, %s, %s, %s, %s, %s, %s, %s)",
                SQLSurrounded(name),
                SQLSurrounded(surname),
                SQLSurrounded(phone),
                SQLSurrounded(otherPhone),
                SQLSurrounded(email),
                SQLSurrounded(address),
                SQLSurrounded(city),
                SQLSurrounded(county),
                SQLSurrounded(postalCode)
                );
    }

    static String SQLSurrounded(String string) {
        if (string.isEmpty ())
            return "NULL"; // not surrounded

        return "'" + string + "'";
    }

    boolean fieldsOk() {
        return  !name.isEmpty () &&
                !surname.isEmpty () &&
                !phone.isEmpty () &&
                !email.isEmpty ();
    }
}
