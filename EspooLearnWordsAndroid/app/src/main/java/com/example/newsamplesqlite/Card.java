package com.example.newsamplesqlite;

public class Card {
    private long id;
    private String wordEnglish;
    private String wordPronun;
    private String wordForeign;
    private int color_resource;
    private int listPosition = 0;

    public Card() {
    }

    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getWordEnglish() {
        return wordEnglish;
    }

    public void setWordEnglish(String name) {
        this.wordEnglish = name;
    }

    public String getWordPronun() {
        return wordPronun;
    }

    public void setWordPronun(String pronun) {
        this.wordPronun = pronun;
    }

    public String getWordForeign() {
        return wordForeign;
    }

    public void setWordForeign(String word) {
        this.wordForeign = word;
    }

    public int getColorResource() {
        return color_resource;
    }

    public void setColorResource(int color_resource) {
        this.color_resource = color_resource;
    }

    public int getListPosition() {
        return listPosition;
    }

    public void setListPosition(int listPosition) {
        this.listPosition = listPosition;
    }
}
