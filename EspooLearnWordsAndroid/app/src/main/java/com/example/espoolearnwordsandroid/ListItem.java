package com.example.espoolearnwordsandroid;

public class ListItem {
    private String polish_word;
    private String english_word;
    private String pronunciation;

    public String getPolishWord()
    {
        return polish_word;
    }
    public void setPolishWord(String pol_word)
    {
        this.polish_word = pol_word;
    }
    public String getEnglishWord()
    {
        return english_word;
    }
    public void setEnglishWord(String eng_word)
    {
        this.english_word = eng_word;
    }
    public String getPronunciation()
    {
        return pronunciation;
    }

    public void setPronunciation(String pronun)
    {
        this.pronunciation = pronun;
    }
}
